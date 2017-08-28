// king-spi.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "king-spi.h"
#include "singleton_configure.h"
#include "singleton_filter.h"

//ȫ��׃�� ���� ϵ�y ԭ�����ṩ�ߵ� ���F
WSPPROC_TABLE ProcTable;

int WSPAPI WSPStartup(
	__in   WORD wVersionRequested,
	__out  LPWSPDATA lpWSPData,
	__in   LPWSAPROTOCOL_INFO lpProtocolInfo,
	__in   WSPUPCALLTABLE UpcallTable,
	__out  LPWSPPROC_TABLE lpProcTable
)
{
	KG_INFO("WSPStartup");

	try
	{
		packed_catalog_items_t items;
		//�]�� ���b��Ϣ
		if(!install_t::is_install(items))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//�� %XXX% �h��׃�� ������·��
		char path[MAX_PATH] = {0};
		if(!ExpandEnvironmentStringsA(items[0]->item.Path, path, MAX_PATH))
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		
		//���d dll
		HMODULE moudle = LoadLibraryA(path);
		if(!moudle)
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		//�@ȡ ��ʼ�� ����
		LPWSPSTARTUP WSPStartupFunc = (LPWSPSTARTUP)GetProcAddress(moudle,"WSPStartup");
		if(!WSPStartupFunc)
		{
			FreeLibrary(moudle);
			return WSAEPROVIDERFAILEDINIT;
		}

		int rs = WSPStartupFunc(wVersionRequested,lpWSPData,lpProtocolInfo,UpcallTable,lpProcTable);
		if(rs)
		{
			return rs;
		}

		//��Ҫ hook �W�j
		if(singleton_configure::get_const_instance().ok())
		{
			//���� ԭ���F
			ProcTable = *lpProcTable;
		
			//Ҫ�r�ص� ���F
			lpProcTable->lpWSPAccept = WSPAccept;
			lpProcTable->lpWSPCloseSocket = WSPCloseSocket;
			lpProcTable->lpWSPRecv = WSPRecv;
		}
	}
	catch(const boost::system::system_error& e)
	{
		KG_FAULT(e.what());
		return WSAEPROVIDERFAILEDINIT;
	}
	return 0;
}
SOCKET WSPAPI WSPAccept(
    __in SOCKET s,
    __out_bcount_part_opt(*addrlen, *addrlen) struct sockaddr FAR * addr,
    __inout_opt LPINT addrlen,
    __in_opt LPCONDITIONPROC lpfnCondition,
    __in_opt DWORD_PTR dwCallbackData,
    __out LPINT lpErrno
)
{
	KG_INFO("WSPAccept");
	SOCKET c = ProcTable.lpWSPAccept(s,addr,addrlen,lpfnCondition,dwCallbackData,lpErrno);
	if(c != INVALID_SOCKET &&
		!singleton_filter::get_mutable_instance().request_safe(s)
		)
	{
		ProcTable.lpWSPCloseSocket(c,NULL);
		return INVALID_SOCKET;
	}
	return c;
}

int WSPAPI WSPCloseSocket(
  __in   SOCKET s,
  __out  LPINT lpErrno
)
{
	KG_INFO("WSPCloseSocket");
	return ProcTable.lpWSPCloseSocket(s,lpErrno);
}
int WSPAPI WSPRecv(
  __in     SOCKET s,
  __inout  LPWSABUF lpBuffers,
  __in     DWORD dwBufferCount,
  __out    LPDWORD lpNumberOfBytesRecvd,
  __inout  LPDWORD lpFlags,
  __in     LPWSAOVERLAPPED lpOverlapped,
  __in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in     LPWSATHREADID lpThreadId,
  __out    LPINT lpErrno
)
{
	KG_INFO("WSPRecv");
	SOCKADDR_IN addr;
	INT len = sizeof(addr);
	if(!ProcTable.lpWSPGetPeerName(s,(sockaddr*)&addr,&len,NULL))
	{
		u_short port = ntohs((u_short)addr.sin_addr.s_addr);
		char* ip = inet_ntoa(addr.sin_addr);
		std::cout<<"recv "<<ip<<":"<<port<<"\n";
	}

	return ProcTable.lpWSPRecv(s
		,lpBuffers
		,dwBufferCount
		,lpNumberOfBytesRecvd
		,lpFlags
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}

