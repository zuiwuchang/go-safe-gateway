#pragma once
class singleton_tcp
	:public kg::utils::singleton<singleton_tcp>
{
public:
	singleton_tcp(void);
	~singleton_tcp(void);

	//���ip
	void post_disable(const std::string& ip)
	{
	}
};

