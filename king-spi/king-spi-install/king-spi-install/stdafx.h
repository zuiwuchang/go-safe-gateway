// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>

#include <WinSock2.h>

// TODO: 在此处引用程序需要的其他头文件
#include <boost/program_options.hpp>
#include <boost/system/system_error.hpp>
#include <boost/exception/all.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#define CREATE_CATEGORY(Category,NAME) class Category:public boost::system::error_category{public:\
	typedef Category type_t;\
	private:\
	static type_t& _get(){static type_t instance;return instance;}\
	std::string _emsg;public:\
	virtual const char *name() const BOOST_SYSTEM_NOEXCEPT{return NAME;}\
	virtual std::string message(int ev) const\
    {\
    	std::string msg(NAME);\
		msg += " : ";\
        switch(ev)\
        {\
        case 0:\
            return msg + "success";\
		default:\
            return msg + _emsg;\
        }\
        return msg + "unknow";\
    }\
	static type_t& get()\
    {\
    	return _get();\
    }\
	static type_t& get(const std::string& emsg)\
    {\
		type_t& instance = _get();\
		instance._emsg = emsg;\
    	return instance;\
    }\
};

#include <kg/windows/utf.hpp>