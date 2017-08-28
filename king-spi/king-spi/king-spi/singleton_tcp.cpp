#include "StdAfx.h"
#include "singleton_tcp.h"
#include "singleton_configure.h"

#define HEADER_FLAG	731
#define	HEADER_SIZE	6

//��� ip
#define COMMAND_REJECT	200

//ping
#define COMMAND_PING	800
#define COMMAND_PING_NO	801

//ping �g��
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

	//Ͷ�f�µ� ping �΄�
	_timer.expires_from_now(PING_INTERVAL);
	_timer.async_wait(boost::bind(&singleton_tcp::handler_timer,this,_1));

	//�]�н����B�� ����Ҫ ping
	if(!_c)
	{
		return;
	}

	//���� ping
	try
	{
		kg::byte_t buffer[HEADER_SIZE];
		fmt_header(buffer,HEADER_SIZE,COMMAND_PING_NO);
		_c->get().write_some(boost::asio::buffer(buffer,HEADER_SIZE));
	}
	catch(const boost::system::system_error&)
	{
		//�l�� ping ʧ�� �B����ʧЧ
		//ጷ� �B��
		_c.reset();
	}
}
void singleton_tcp::work()
{
	_service.run();
}
//���ip
void singleton_tcp::post_disable(const std::string& ip)
{
	boost::mutex::scoped_lock(_mutex);
	//�ѽ��� ����� ֱ�ӷ���
	if(_requests.end() != _requests.find(ip))
	{
		return;
	}

	//Ո��̎��
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
			//��� ʧ�� ���� ������ δ���� �ȴ� ������ ����
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	}
}
bool singleton_tcp::get_ip(std::string& ip)
{
	boost::mutex::scoped_lock(_mutex);
	//�]�� ��̎�� ip
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
	//�B���ѽ���
	if(_c)
	{
		if(write_request(ip))
		{
			return true;
		}
	}

	//���� �B�� 
	return create_client() &&
		write_request(ip);	//�l��Ո��
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
	//�B��ʧ�� ጷ��YԴ
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

		//�l��Ո��
		_c->get().write_some(boost::asio::buffer(buffer.get(),n));

		//�ȴ��ؑ�
		client_t::bytes_t bytes = _c->read();

		//�e�`�� �؂�ֵ
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
	//Ո��ʧ�� ጷ��YԴ
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