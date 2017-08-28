// king-spi.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "king-spi.h"
#include "singleton_configure.h"
#include "singleton_filter.h"
#include "singleton_ex.h"
#include "singleton_tcp.h"

//ȫ��׃�� ���� ϵ�y ԭ�����ṩ�ߵ� ���F
WSPPROC_TABLE ProcTable;
GUID guidAcceptEx = WSAID_ACCEPTEX;

int WSPAPI WSPStartup(
	__in   WORD wVersionRequested,
	__out  LPWSPDATA lpWSPData,
	__in   LPWSAPROTOCOL_INFO lpProtocolInfo,
	__in   WSPUPCALLTABLE UpcallTable,
	__out  LPWSPPROC_TABLE lpProcTable
)
{
	KG_INFO("WSPStartup");
	std::ofstream outf("c:/logs.txt",std::ios::app|std::ios::out);
	if(outf.is_open())
	{
		char path[MAX_PATH] = {0};
		GetModuleFileNameA(NULL,path,MAX_PATH);
		outf<<GetCurrentProcessId()<<"   "<<path<<std::endl;
	}

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

			//��ʼ�� �^�^�V��
			singleton_filter::get_mutable_instance().init();
			//��ʼ�� AcceptEx ӛ�
			singleton_ex::get_mutable_instance().init(&ProcTable);
			//��ʼ�� ֪ͨ����
			singleton_tcp::get_mutable_instance().init();
			
		
			//Ҫ�r�ص� ���F
			lpProcTable->lpWSPAccept = WSPAccept;
			lpProcTable->lpWSPCloseSocket = WSPCloseSocket;
			lpProcTable->lpWSPIoctl = WSPIoctl;
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
	KG_TRACE("WSPAccept");
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
	KG_INFO("WSPCloseSocket "<<s);
	singleton_ex::get_mutable_instance().erase(s);
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
	KG_TRACE("WSPRecv "<<s);
	if(!singleton_ex::get_mutable_instance().request_safe(s))
	{
		*lpErrno = WSAEFAULT;
		return SOCKET_ERROR;
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

int WSPAPI WSPIoctl(
  __in   SOCKET s,
  __in   DWORD dwIoControlCode,
  __in   LPVOID lpvInBuffer,
  __in   DWORD cbInBuffer,
  __out  LPVOID lpvOutBuffer,
  __in   DWORD cbOutBuffer,
  __out  LPDWORD lpcbBytesReturned,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in   LPWSATHREADID lpThreadId,
  __out  LPINT lpErrno
)
{
	KG_TRACE("WSPIoctl");
	
	if(SIO_GET_EXTENSION_FUNCTION_POINTER == dwIoControlCode)
	{
		if(0 == memcmp(lpvInBuffer,&guidAcceptEx,sizeof(GUID)))
		{
			if (cbOutBuffer < sizeof(LPFN_ACCEPTEX))
			{
				*lpcbBytesReturned = sizeof(LPFN_ACCEPTEX);
				*lpErrno = WSAEFAULT;
				return SOCKET_ERROR;
			}

			KG_TRACE("get AcceptEx");
			int rs = singleton_ex::get_mutable_instance().AcceptEx(s
				,dwIoControlCode
				,lpvInBuffer
				,cbInBuffer
				,lpvOutBuffer
				,cbOutBuffer
				,lpcbBytesReturned
				,lpOverlapped
				,lpCompletionRoutine
				,lpThreadId
				,lpErrno
				);
			if(rs == NO_ERROR)
			{
				*lpcbBytesReturned = sizeof( LPFN_ACCEPTEX );
				*((DWORD_PTR *)lpvOutBuffer) = (DWORD_PTR) ExtAcceptEx;
			}
			return rs;
		}
	}

	return ProcTable.lpWSPIoctl(s
		,dwIoControlCode
		,lpvInBuffer
		,cbInBuffer
		,lpvOutBuffer
		,cbOutBuffer
		,lpcbBytesReturned
		,lpOverlapped
		,lpCompletionRoutine
		,lpThreadId
		,lpErrno
		);
}
BOOL PASCAL FAR ExtAcceptEx(
    __in SOCKET sListenSocket,
    __in SOCKET sAcceptSocket,
    __in PVOID lpOutputBuffer,
    __in DWORD dwReceiveDataLength,
    __in DWORD dwLocalAddressLength,
    __in DWORD dwRemoteAddressLength,
    __out LPDWORD lpdwBytesReceived,
    __inout LPOVERLAPPED lpOverlapped
    )
{
	KG_TRACE("AcceptEx "<<sAcceptSocket);
	LPFN_ACCEPTEX acceptEx = singleton_ex::get_mutable_instance().AcceptEx();
	BOOL rs = acceptEx(sListenSocket
		,sAcceptSocket
		,lpOutputBuffer
		,dwReceiveDataLength
		,dwLocalAddressLength
		,dwRemoteAddressLength
		,lpdwBytesReceived
		,lpOverlapped
		);
	singleton_ex::get_mutable_instance().insert(sAcceptSocket);
	return rs;
}