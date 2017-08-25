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
	//返回是否需要 劫持
	//如果 需要 將 端口 修改爲 劫持端口
	bool hijack(u_short& port)const;
};

