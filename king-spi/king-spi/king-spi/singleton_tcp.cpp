#include "StdAfx.h"
#include "singleton_tcp.h"
#include "singleton_configure.h"

#define HEADER_FLAG	731
#define	HEADER_SIZE	6

//查封 ip
#define COMMAND_REJECT	200

//ping
#define COMMAND_PING	800
#define COMMAND_PING_NO	801

//ping 間隔
#define PING_INTERVAL	(boost::posix_time::minutes(30))


singleton_tcp::singleton_tcp(void):_work(_service),_timer(_service)
{
}


singleton_tcp::~singleton_tcp(void)
{
}
void singleton_tcp::init()
{
	_timer.expires_from_now(PING_INTERVAL);
	_timer.async_wait(boost::bind(&singleton_tcp::handler_timer,this,_1));
	boost::thread(boost::bind(&singleton_tcp::work,this));
}
void singleton_tcp::handler_timer(const boost::system::error_code& e)
{
	if(e)
	{
		return;
	}

	//投遞新的 ping 任務
	_timer.expires_from_now(PING_INTERVAL);
	_timer.async_wait(boost::bind(&singleton_tcp::handler_timer,this,_1));

	//沒有建立連接 不需要 ping
	if(!_c)
	{
		return;
	}

	//執行 ping
	try
	{
		kg::byte_t buffer[HEADER_SIZE];
		fmt_header(buffer,HEADER_SIZE,COMMAND_PING_NO);
		_c->get().write_some(boost::asio::buffer(buffer,HEADER_SIZE));
	}
	catch(const boost::system::system_error&)
	{
		//發送 ping 失敗 連接已失效
		//釋放 連接
		_c.reset();
	}
}
void singleton_tcp::work()
{
	_service.run();
}
//查封ip
void singleton_tcp::post_disable(const std::string& ip)
{
	boost::mutex::scoped_lock(_mutex);
	//已經在 隊列中 直接返回
	if(_requests.end() != _requests.find(ip))
	{
		return;
	}

	//請求處理
	_requests.insert(ip);
	_service.post(boost::bind(&singleton_tcp::handler_post,this));
}
void singleton_tcp::handler_post()
{
	std::string ip;
	while(get_ip(ip))
	{
		while(!disable_ip(ip))
		{
			//查封 失敗 可能 服務器 未啓動 等待 服務器 啓動
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	}
}
bool singleton_tcp::get_ip(std::string& ip)
{
	boost::mutex::scoped_lock(_mutex);
	//沒有 待處理 ip
	if(_requests.empty())
	{
		return false;
	}

	BOOST_AUTO(iter,_requests.begin());
	ip = *iter;
	_requests.erase(iter);
	return true;
}
bool singleton_tcp::disable_ip(std::string& ip)
{
	//連接已建立
	if(_c)
	{
		if(write_request(ip))
		{
			return true;
		}
	}

	//建立 連接 
	return create_client() &&
		write_request(ip);	//發送請求
}
int singleton_tcp::_reader(kg::byte_t* b,std::size_t n)
{
	if(HEADER_SIZE != n)
	{
		return 0;
	}

	kg::uint16_t* p = (kg::uint16_t*)b;

	//flag
	if(HEADER_FLAG != *p)
	{
		return 0;
	}

	//len
	kg::uint16_t len = *(p + 1);

	return int(len);
}
bool singleton_tcp::create_client()
{

	try
	{
		_c = boost::make_shared<client_t>(HEADER_SIZE);
		_c->reader(_reader);
		_c->connect(singleton_configure::get_const_instance().safe().addr);

		return true;
	}
	catch(const std::bad_alloc&)
	{
	}
	catch(const boost::system::system_error&)
	{
	}
	//連接失敗 釋放資源
	_c.reset();
	return false;
}
bool singleton_tcp::write_request(std::string& ip)
{
	std::size_t n = HEADER_SIZE + ip.size();
	boost::shared_array<kg::byte_t> buffer;
	try
	{
		buffer = boost::shared_array<kg::byte_t>(new kg::byte_t[n]);
	}
	catch(const std::bad_alloc&)
	{
		return false;
	}

	try
	{
		//header
		fmt_header(buffer.get(),n,COMMAND_REJECT);
		std::copy(ip.begin(),ip.end(),buffer.get() + HEADER_SIZE);

		//發送請求
		_c->get().write_some(boost::asio::buffer(buffer.get(),n));

		//等待回應
		client_t::bytes_t bytes = _c->read();

		//錯誤的 回傳值
		if(bytes.size() != HEADER_SIZE)
		{
			KG_FAULT("bad reject rs len");
			return false;
		}
		kg::uint16_t* p = (kg::uint16_t*)(bytes.get());
		if(COMMAND_REJECT != *(p + 2))
		{
			KG_FAULT("bad reject rs cmd");
			return false;
		}
		return true;
	}
	catch(boost::system::system_error&)
	{

	}
	//請求失敗 釋放資源
	_c.reset();
	return false;
}
void singleton_tcp::fmt_header(kg::byte_t* b,kg::uint16_t len,kg::uint16_t cmd)
{
	kg::uint16_t* p = (kg::uint16_t*)b;
	*p = HEADER_FLAG;
	*(p + 1) = len;
	*(p + 2) = cmd;
}