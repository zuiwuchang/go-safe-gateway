#include "StdAfx.h"
#include "singleton_configure.h"
#include "funcs.h"

#define CONFIGURE_FILE	L"king-spi.json"

singleton_configure::singleton_configure(void)
	:_ok(false)
{
}


singleton_configure::~singleton_configure(void)
{
}
void singleton_configure::init(HMODULE hModule)
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(hModule,buffer,MAX_PATH);
	boost::filesystem::wpath path(buffer);
	path = path.parent_path() / CONFIGURE_FILE; 
	const std::string configureFile = path.string();
	KG_TRACE("cnf path : "<<configureFile);
	
	GetModuleFileName(NULL,buffer,MAX_PATH);
	path = buffer;
	std::string processPath = TO_UTF8(path.wstring());
	std::string processName = TO_UTF8(path.filename().wstring());
	KG_TRACE("process path : "<<UTF8_TO_CHAR(processPath));
	KG_TRACE("process name : "<<UTF8_TO_CHAR(processName));

	//windows 忽略檔案名 大小寫
	boost::algorithm::to_lower(processPath);
	boost::algorithm::to_lower(processName);
	try
	{
		boost::property_tree::ptree tree;
		boost::property_tree::read_json(configureFile,tree);

		//apps
		auto& childs = tree.get_child("Apps");
		std::string matchName;
		BOOST_FOREACH(auto& node,childs)
		{
			std::string name = node.second.get<std::string>("Name");
			//全部進程
			if("*" == name)
			{
				_ok = true;
				break;
			}
			boost::algorithm::to_lower(name);

			//使用 路徑匹配
			if(node.second.get<bool>("Path",false))
			{
				matchName = processPath;
			}
			else
			{
				matchName = processName;
			}

			//是否使用 正則 匹配
			if(node.second.get<bool>("Regex",false))
			{
				boost::xpressive::sregex reg = boost::xpressive::sregex::compile(name);
				if(boost::xpressive::regex_match(matchName,reg))
				{
					_ok = true;
					break;
				}
			}
			else
			{
				if(matchName == name)
				{
					_ok = true;
					break;
				}
			}
		}
		//safe
		_safe.num = tree.get<std::size_t>("Safe.Num",100);
		_safe.duration = boost::posix_time::milliseconds(tree.get<int64_t>("Safe.Duration",1000));
		_safe.disable = boost::posix_time::hours(tree.get<long>("Safe.Disable",48));
		_safe.addr = tree.get<std::string>("Safe.Addr","127.0.0.1:6661");
	}
	catch(const std::exception& e)
	{
		KG_ERROR(e.what());
	}

	KG_TRACE_SECTION(
		if(_ok)
		{
			_safe.show();
		}
	);
}