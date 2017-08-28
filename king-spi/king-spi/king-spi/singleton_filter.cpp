#include "StdAfx.h"
#include "singleton_filter.h"
#include "singleton_tcp.h"

extern WSPPROC_TABLE ProcTable;

singleton_filter::singleton_filter(void)
	:_work(_service)
{
}


singleton_filter::~singleton_filter(void)
{
}
void singleton_filter::init()
{
	boost::thread(boost::bind(&singleton_filter::work,this));
}
void singleton_filter::work()
{
	_service.run();
}
void singleton_filter::handler(const boost::system::error_code& e,deadline_timer_spt timer,std::string ip)
{
	if(e)
	{
		return;
	}

	boost::mutex::scoped_lock(_mutex);
	BOOST_AUTO(findBlacks,_blacks.find(ip));
	if(findBlacks != _blacks.end())
	{
		_blacks.erase(findBlacks);
	}
}
bool singleton_filter::request_safe(SOCKET s)
{
	SOCKADDR_IN addr;
	INT len = sizeof(addr);
	if(!ProcTable.lpWSPGetPeerName(s,(sockaddr*)&addr,&len,NULL))
	{
		return false;
	}
	const std::string ip = inet_ntoa(addr.sin_addr);
	
	
	boost::mutex::scoped_lock(_mutex);
	KG_TRACE("request_safe "<<s<<" "<<ip);
	//在黑名單中 直接返回 失敗
	BOOST_AUTO(findBlacks,_blacks.find(ip));
	if(findBlacks != _blacks.end())
	{
		//通知 安全 服務器
		singleton_tcp::get_mutable_instance().post_disable(ip);
		return false;
	}

	//第一次連接
	auto find = _nodes.find(ip);
	if(find == _nodes.end())
	{
		try
		{
			_nodes[ip] = boost::make_shared<filter_node_t>();
		}
		catch(const std::bad_alloc&)
		{
		}
		return true;
	}

	//驗證是否是 安全連接
	if(find->second->update_safe())
	{
		return true;
	}

	//非法 查封ip
	_blacks.insert(ip);
	//創建 解封 任務
	const singleton_configure& cnf = singleton_configure::get_const_instance();
	deadline_timer_spt timer = boost::make_shared<deadline_timer_t>(_service,cnf.safe().disable);
	timer->async_wait(boost::bind(&singleton_filter::handler,this,_1,timer,ip));
	//通知 安全 服務器
	singleton_tcp::get_mutable_instance().post_disable(ip);
	return false;
}