#pragma once
class singleton_tcp
	:public kg::utils::singleton<singleton_tcp>
{
public:
	singleton_tcp(void);
	~singleton_tcp(void);

	//²é·âip
	void post_disable(const std::string& ip)
	{
	}
};

