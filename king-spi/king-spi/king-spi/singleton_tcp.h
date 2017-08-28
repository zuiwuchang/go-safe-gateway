#pragma once
class singleton_tcp
	:public kg::utils::singleton<singleton_tcp>
{
public:
	singleton_tcp(void);
	~singleton_tcp(void);

	void init();
private:
	boost::mutex _mutex;
	boost::unordered_set<std::string> _requests;

	boost::asio::io_service _service;
	boost::asio::io_service::work _work;
	void work();
	boost::asio::deadline_timer _timer;
	void handler_timer(const boost::system::error_code& e);
	
public:
	void post_disable(const std::string& ip);
private:
	void handler_post();
	//返回一個需要 處理的 ip
	bool get_ip(std::string& ip);
	//向服務器 請求 查封 ip
	bool disable_ip(std::string& ip);
	typedef kg::net::echo_client_t::type_t client_t;
	typedef kg::net::echo_client_t::type_spt client_spt;
	client_spt _c;
	bool write_request(std::string& ip);
	static int _reader(kg::byte_t* b,std::size_t n);
	static void fmt_header(kg::byte_t* b,kg::uint16_t len,kg::uint16_t cmd);
	bool create_client();
};

