// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
			//���d����
			singleton_configure& cnf = singleton_configure::get_mutable_instance();
			cnf.init(hModule);

			//��ʼ�� �^�^�V��
			singleton_filter::get_mutable_instance().init();
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

