#pragma once
//��ȫ�O��
class safe_t
	:boost::noncopyable
{
public:
	std::size_t num;
	boost::posix_time::time_duration duration;
	boost::posix_time::time_duration disable;
	std::string addr;
	
	safe_t()
	{
	}
	void show()
	{
		std::cout<<"***	safe	***\n"
			<<"num = "<<num<<"\n"
			<<"duration = "<<duration<<"\n"
			<<"disable = "<<disable<<"\n"
			<<"addr = "<<addr<<"\n"
			<<"***	safe	***\n";
	}
};
class singleton_configure
	:public kg::utils::singleton<singleton_configure>
{
public:
	singleton_configure(void);
	~singleton_configure(void);

	void init(HMODULE hModule);
private:
	//��ȫ�O��
	safe_t _safe;
	//�Ƿ���Ҫ �O��
	bool _ok;
public:
	//�����Ƿ� ��Ҫ��ȫ�O��
	inline bool ok()const
	{
		return _ok;
	}
	const safe_t& safe()const
	{
		return _safe;
	}
};

