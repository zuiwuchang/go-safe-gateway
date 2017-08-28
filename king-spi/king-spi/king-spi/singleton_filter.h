#pragma once
#include "singleton_configure.h"
struct filter_node_t
{
	//最後連接時間
	boost::posix_time::ptime last;
	//連接次數
	std::size_t num;

	filter_node_t()
		:num(1),last(boost::posix_time::second_clock::local_time())
	{
	}

	//返回 是否是 安全連接
	bool update_safe()
	{
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		const safe_t& safe = singleton_configure::get_const_instance().safe();

		//時間安全
		if(now > (last + safe.duration))
		{
			num = 1;
			last = now;
			return true;
		}

		//連接次數 安全
		if(num < safe.num)
		{
			++num;
			last = now;
			return true;
		}
		return false;
	}
};
typedef boost::shared_ptr<filter_node_t> filter_node_spt;
typedef boost::asio::deadline_timer  deadline_timer_t;
typedef boost::shared_ptr<deadline_timer_t> deadline_timer_spt;
class singleton_filter
	: public kg::utils::singleton<singleton_filter>
{
public:
	singleton_filter(void);
	~singleton_filter(void);

private:
	boost::mutex _mutex;
	//連接記錄
	boost::unordered_map<std::string/*ip*/,filter_node_spt> _nodes;

	//黑名單
	boost::unordered_set<std::string/*ip*/> _blacks;

	boost::asio::io_service _service;
	boost::asio::io_service::work _work;
	void work();
	void handler(const boost::system::error_code& e,deadline_timer_spt timer,std::string ip);
public:
	void init();
	//更新連接記錄 並返回是否是安全連接
	bool request_safe(SOCKET s);
};

