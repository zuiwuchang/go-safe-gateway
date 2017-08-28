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
	//�ں������� ֱ�ӷ��� ʧ��
	BOOST_AUTO(findBlacks,_blacks.find(ip));
	if(findBlacks != _blacks.end())
	{
		//֪ͨ ��ȫ ������
		singleton_tcp::get_mutable_instance().post_disable(ip);
		return false;
	}

	//��һ���B��
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

	//��C�Ƿ��� ��ȫ�B��
	if(find->second->update_safe())
	{
		return true;
	}

	//�Ƿ� ���ip
	_blacks.insert(ip);
	//���� ��� �΄�
	const singleton_configure& cnf = singleton_configure::get_const_instance();
	deadline_timer_spt timer = boost::make_shared<deadline_timer_t>(_service,cnf.safe().disable);
	timer->async_wait(boost::bind(&singleton_filter::handler,this,_1,timer,ip));
	//֪ͨ ��ȫ ������
	singleton_tcp::get_mutable_instance().post_disable(ip);
	return false;
}