#include "StdAfx.h"
#include "singleton_cnf.h"

#define CONFIGURE_FILE	"ks2_32.json"
singleton_cnf::singleton_cnf(void)
{
}


singleton_cnf::~singleton_cnf(void)
{
}
void singleton_cnf::init()
{
	KG_TRACE("init");
	char str[MAX_PATH];
	GetModuleFileNameA(NULL,str,MAX_PATH);
	boost::filesystem::path path(str);
	path = path.parent_path() / CONFIGURE_FILE;
	std::string cnf = path.string();
	KG_TRACE(cnf);
	load(cnf);
}
void singleton_cnf::load(const std::string& path)
{
	try
	{
		boost::property_tree::ptree tree;
		boost::property_tree::read_json(path,tree);

		typedef BOOST_TYPEOF(*tree.begin()) ptree_t;
		BOOST_FOREACH(ptree_t& node,tree)
		{
			u_short src = node.second.get<u_short>("Src",0);
			u_short dist = node.second.get<u_short>("Dist",0);
			
			if(src && dist)
			{
				KG_INFO("hijack "<<src<<" to "<<dist);
				_ports[src] = dist;

			}
		}
	}
	catch(const std::exception& e)
	{
		KG_FAULT(e.what());
	}
}
bool singleton_cnf::hijack(u_short& port)const
{
	BOOST_AUTO(find,_ports.find(port));
	if(find == _ports.end())
	{
		return false;
	}

	port = find->second;
	return true;

}