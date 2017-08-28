// king-spi-install.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "install.hpp"

#define KING_VERSION		"1.0.1"


int main(int argc, char* argv[])
{
	//define console params
	boost::program_options::options_description opts("options");
	opts.add_options()
		("help,h","show help")
		("install,i","install")
		("uninstall,u","uninstall")
		("version,v","show version")
		;
	int rs = 0;
	try
	{
		//analyze
		boost::program_options::variables_map params;
		boost::program_options::store(boost::program_options::parse_command_line(argc,argv,opts),params);
		
		//get params
		if(params.count("version"))
		{
			std::cout<<KING_VERSION<<std::endl;
		}
		else if(params.count("install"))
		{
			try
			{
				install_t::install();
			}
			catch(const boost::system::system_error& e)
			{
				std::cout<< boost::diagnostic_information(e)<<std::endl;
				install_t::uninstall();
				return 1;
			}
		}
		else if(params.count("uninstall"))
		{
			install_t::uninstall();
		}
		else
		{
			std::cout<<opts<<std::endl;
		}
	}
	catch(const boost::system::system_error& e)
	{
		rs = 1;
		std::cout<< boost::diagnostic_information(e)<<std::endl;
		std::system("pause");
	}
	return rs;
}