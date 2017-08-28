// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "singleton_configure.h"
#include "singleton_filter.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//加載配置
			singleton_configure& cnf = singleton_configure::get_mutable_instance();
			cnf.init(hModule);
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

