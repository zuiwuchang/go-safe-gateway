#pragma once
#include "singleton.hpp"
class singleton_cnf
	:public singleton<singleton_cnf>
{
public:
	singleton_cnf(void);
	~singleton_cnf(void);
	void init();
private:
	void load(const std::string& path);
	boost::unordered_map<u_short,u_short> _ports;

public:
	//�����Ƿ���Ҫ �ٳ�
	//��� ��Ҫ �� �˿� �޸Ġ� �ٳֶ˿�
	bool hijack(u_short& port)const;
};

