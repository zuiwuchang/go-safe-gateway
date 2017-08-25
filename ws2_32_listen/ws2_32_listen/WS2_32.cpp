#include "stdafx.h"
#include "singleton_cnf.h"

#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define HIJACK_CODE EXTERNC NAKED void __cdecl

//dll main
//原dll 句柄
HMODULE g_hModule = NULL;
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//禁用 DLL_THREAD_ATTACH DLL_THREAD_DETACH 通知
		DisableThreadLibraryCalls(hModule);

		//請 修改為正確 路徑
		g_hModule = LoadLibrary(L"C:/Windows/System32/ws2_32.dll");
		if(g_hModule == NULL)
		{
			MessageBox(NULL, L"加載dll失敗-C:/Windows/System32/ws2_32.dll", L"錯誤", MB_ICONSTOP);
			ExitProcess(-1);
		}
#ifdef _DEBUG
		kg::windows::console::alloc();
#endif
		singleton_cnf::get_mutable_instance().init();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		if(g_hModule != NULL)
		{
			FreeLibrary(g_hModule);
			g_hModule = NULL;
		}
	}
	return TRUE;
}


//地址獲取
FARPROC WINAPI GetAddress(PCSTR name)
{
		if(g_hModule == NULL)
		{
			return 0;
		}
		return GetProcAddress(g_hModule, name);
}

int WINAPI filter_bind(SOCKET s,const struct sockaddr FAR* name,int namelen)
{
	SOCKADDR_IN* addr = (SOCKADDR_IN*)name;
	u_short port = ntohs(addr->sin_port);

	if(singleton_cnf::get_const_instance().hijack(port))
	{
		KG_INFO("bind "<<port);
		addr->sin_port = htons(port);
	}
	return bind(s,name,namelen);
}
FARPROC WINAPI GetAddressBind()
{
	return (FARPROC)filter_bind;
}

//轉發 原函數
HIJACK_CODE My_accept(void)
{
	GetAddress("accept");
	__asm JMP EAX;
}
HIJACK_CODE My_bind(void)
{
	//GetAddress("bind");
	GetAddressBind();
	__asm JMP EAX;
}
HIJACK_CODE My_closesocket(void)
{
	GetAddress("closesocket");
	__asm JMP EAX;
}
HIJACK_CODE My_connect(void)
{
	GetAddress("connect");
	__asm JMP EAX;
}
HIJACK_CODE My_getpeername(void)
{
	GetAddress("getpeername");
	__asm JMP EAX;
}
HIJACK_CODE My_getsockname(void)
{
	GetAddress("getsockname");
	__asm JMP EAX;
}
HIJACK_CODE My_getsockopt(void)
{
	GetAddress("getsockopt");
	__asm JMP EAX;
}
HIJACK_CODE My_htonl(void)
{
	GetAddress("htonl");
	__asm JMP EAX;
}
HIJACK_CODE My_htons(void)
{
	GetAddress("htons");
	__asm JMP EAX;
}
HIJACK_CODE My_ioctlsocket(void)
{
	GetAddress("ioctlsocket");
	__asm JMP EAX;
}
HIJACK_CODE My_inet_addr(void)
{
	GetAddress("inet_addr");
	__asm JMP EAX;
}
HIJACK_CODE My_inet_ntoa(void)
{
	GetAddress("inet_ntoa");
	__asm JMP EAX;
}
HIJACK_CODE My_listen(void)
{
	GetAddress("listen");
	__asm JMP EAX;
}
HIJACK_CODE My_ntohl(void)
{
	GetAddress("ntohl");
	__asm JMP EAX;
}
HIJACK_CODE My_ntohs(void)
{
	GetAddress("ntohs");
	__asm JMP EAX;
}
HIJACK_CODE My_recv(void)
{
	GetAddress("recv");
	__asm JMP EAX;
}
HIJACK_CODE My_recvfrom(void)
{
	GetAddress("recvfrom");
	__asm JMP EAX;
}
HIJACK_CODE My_select(void)
{
	GetAddress("select");
	__asm JMP EAX;
}
HIJACK_CODE My_send(void)
{
	GetAddress("send");
	__asm JMP EAX;
}
HIJACK_CODE My_sendto(void)
{
	GetAddress("sendto");
	__asm JMP EAX;
}
HIJACK_CODE My_setsockopt(void)
{
	GetAddress("setsockopt");
	__asm JMP EAX;
}
HIJACK_CODE My_shutdown(void)
{
	GetAddress("shutdown");
	__asm JMP EAX;
}
HIJACK_CODE My_socket(void)
{
	GetAddress("socket");
	__asm JMP EAX;
}
HIJACK_CODE My_WSApSetPostRoutine(void)
{
	GetAddress("WSApSetPostRoutine");
	__asm JMP EAX;
}
HIJACK_CODE My_FreeAddrInfoEx(void)
{
	GetAddress("FreeAddrInfoEx");
	__asm JMP EAX;
}
HIJACK_CODE My_FreeAddrInfoExW(void)
{
	GetAddress("FreeAddrInfoExW");
	__asm JMP EAX;
}
HIJACK_CODE My_FreeAddrInfoW(void)
{
	GetAddress("FreeAddrInfoW");
	__asm JMP EAX;
}
HIJACK_CODE My_GetAddrInfoExA(void)
{
	GetAddress("GetAddrInfoExA");
	__asm JMP EAX;
}
HIJACK_CODE My_GetAddrInfoExW(void)
{
	GetAddress("GetAddrInfoExW");
	__asm JMP EAX;
}
HIJACK_CODE My_GetAddrInfoW(void)
{
	GetAddress("GetAddrInfoW");
	__asm JMP EAX;
}
HIJACK_CODE My_GetNameInfoW(void)
{
	GetAddress("GetNameInfoW");
	__asm JMP EAX;
}
HIJACK_CODE My_InetNtopW(void)
{
	GetAddress("InetNtopW");
	__asm JMP EAX;
}
HIJACK_CODE My_InetPtonW(void)
{
	GetAddress("InetPtonW");
	__asm JMP EAX;
}
HIJACK_CODE My_SetAddrInfoExA(void)
{
	GetAddress("SetAddrInfoExA");
	__asm JMP EAX;
}
HIJACK_CODE My_SetAddrInfoExW(void)
{
	GetAddress("SetAddrInfoExW");
	__asm JMP EAX;
}
HIJACK_CODE My_WPUCompleteOverlappedRequest(void)
{
	GetAddress("WPUCompleteOverlappedRequest");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAccept(void)
{
	GetAddress("WSAAccept");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAddressToStringA(void)
{
	GetAddress("WSAAddressToStringA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAddressToStringW(void)
{
	GetAddress("WSAAddressToStringW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAdvertiseProvider(void)
{
	GetAddress("WSAAdvertiseProvider");
	__asm JMP EAX;
}
HIJACK_CODE My_WSACloseEvent(void)
{
	GetAddress("WSACloseEvent");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAConnect(void)
{
	GetAddress("WSAConnect");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAConnectByList(void)
{
	GetAddress("WSAConnectByList");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAConnectByNameA(void)
{
	GetAddress("WSAConnectByNameA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAConnectByNameW(void)
{
	GetAddress("WSAConnectByNameW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSACreateEvent(void)
{
	GetAddress("WSACreateEvent");
	__asm JMP EAX;
}
HIJACK_CODE My_WSADuplicateSocketA(void)
{
	GetAddress("WSADuplicateSocketA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSADuplicateSocketW(void)
{
	GetAddress("WSADuplicateSocketW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumNameSpaceProvidersA(void)
{
	GetAddress("WSAEnumNameSpaceProvidersA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumNameSpaceProvidersExA(void)
{
	GetAddress("WSAEnumNameSpaceProvidersExA");
	__asm JMP EAX;
}
HIJACK_CODE My_gethostbyaddr(void)
{
	GetAddress("gethostbyaddr");
	__asm JMP EAX;
}
HIJACK_CODE My_gethostbyname(void)
{
	GetAddress("gethostbyname");
	__asm JMP EAX;
}
HIJACK_CODE My_getprotobyname(void)
{
	GetAddress("getprotobyname");
	__asm JMP EAX;
}
HIJACK_CODE My_getprotobynumber(void)
{
	GetAddress("getprotobynumber");
	__asm JMP EAX;
}
HIJACK_CODE My_getservbyname(void)
{
	GetAddress("getservbyname");
	__asm JMP EAX;
}
HIJACK_CODE My_getservbyport(void)
{
	GetAddress("getservbyport");
	__asm JMP EAX;
}
HIJACK_CODE My_gethostname(void)
{
	GetAddress("gethostname");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumNameSpaceProvidersExW(void)
{
	GetAddress("WSAEnumNameSpaceProvidersExW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumNameSpaceProvidersW(void)
{
	GetAddress("WSAEnumNameSpaceProvidersW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumNetworkEvents(void)
{
	GetAddress("WSAEnumNetworkEvents");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumProtocolsA(void)
{
	GetAddress("WSAEnumProtocolsA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEnumProtocolsW(void)
{
	GetAddress("WSAEnumProtocolsW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAEventSelect(void)
{
	GetAddress("WSAEventSelect");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetOverlappedResult(void)
{
	GetAddress("WSAGetOverlappedResult");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetQOSByName(void)
{
	GetAddress("WSAGetQOSByName");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetServiceClassInfoA(void)
{
	GetAddress("WSAGetServiceClassInfoA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetServiceClassInfoW(void)
{
	GetAddress("WSAGetServiceClassInfoW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetServiceClassNameByClassIdA(void)
{
	GetAddress("WSAGetServiceClassNameByClassIdA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetServiceClassNameByClassIdW(void)
{
	GetAddress("WSAGetServiceClassNameByClassIdW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAHtonl(void)
{
	GetAddress("WSAHtonl");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAHtons(void)
{
	GetAddress("WSAHtons");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAInstallServiceClassA(void)
{
	GetAddress("WSAInstallServiceClassA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAInstallServiceClassW(void)
{
	GetAddress("WSAInstallServiceClassW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAIoctl(void)
{
	GetAddress("WSAIoctl");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAJoinLeaf(void)
{
	GetAddress("WSAJoinLeaf");
	__asm JMP EAX;
}
HIJACK_CODE My_WSALookupServiceBeginA(void)
{
	GetAddress("WSALookupServiceBeginA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSALookupServiceBeginW(void)
{
	GetAddress("WSALookupServiceBeginW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSALookupServiceEnd(void)
{
	GetAddress("WSALookupServiceEnd");
	__asm JMP EAX;
}
HIJACK_CODE My_WSALookupServiceNextA(void)
{
	GetAddress("WSALookupServiceNextA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSALookupServiceNextW(void)
{
	GetAddress("WSALookupServiceNextW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSANSPIoctl(void)
{
	GetAddress("WSANSPIoctl");
	__asm JMP EAX;
}
HIJACK_CODE My_WSANtohl(void)
{
	GetAddress("WSANtohl");
	__asm JMP EAX;
}
HIJACK_CODE My_WSANtohs(void)
{
	GetAddress("WSANtohs");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAPoll(void)
{
	GetAddress("WSAPoll");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAProviderCompleteAsyncCall(void)
{
	GetAddress("WSAProviderCompleteAsyncCall");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAProviderConfigChange(void)
{
	GetAddress("WSAProviderConfigChange");
	__asm JMP EAX;
}
HIJACK_CODE My_WSARecv(void)
{
	GetAddress("WSARecv");
	__asm JMP EAX;
}
HIJACK_CODE My_WSARecvDisconnect(void)
{
	GetAddress("WSARecvDisconnect");
	__asm JMP EAX;
}
HIJACK_CODE My_WSARecvFrom(void)
{
	GetAddress("WSARecvFrom");
	__asm JMP EAX;
}
HIJACK_CODE My_WSARemoveServiceClass(void)
{
	GetAddress("WSARemoveServiceClass");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAResetEvent(void)
{
	GetAddress("WSAResetEvent");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASend(void)
{
	GetAddress("WSASend");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASendDisconnect(void)
{
	GetAddress("WSASendDisconnect");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASendMsg(void)
{
	GetAddress("WSASendMsg");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASendTo(void)
{
	GetAddress("WSASendTo");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASetEvent(void)
{
	GetAddress("WSASetEvent");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASetServiceA(void)
{
	GetAddress("WSASetServiceA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASetServiceW(void)
{
	GetAddress("WSASetServiceW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASocketA(void)
{
	GetAddress("WSASocketA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASocketW(void)
{
	GetAddress("WSASocketW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncSelect(void)
{
	GetAddress("WSAAsyncSelect");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncGetHostByAddr(void)
{
	GetAddress("WSAAsyncGetHostByAddr");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncGetHostByName(void)
{
	GetAddress("WSAAsyncGetHostByName");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncGetProtoByNumber(void)
{
	GetAddress("WSAAsyncGetProtoByNumber");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncGetProtoByName(void)
{
	GetAddress("WSAAsyncGetProtoByName");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncGetServByPort(void)
{
	GetAddress("WSAAsyncGetServByPort");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAAsyncGetServByName(void)
{
	GetAddress("WSAAsyncGetServByName");
	__asm JMP EAX;
}
HIJACK_CODE My_WSACancelAsyncRequest(void)
{
	GetAddress("WSACancelAsyncRequest");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASetBlockingHook(void)
{
	GetAddress("WSASetBlockingHook");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAUnhookBlockingHook(void)
{
	GetAddress("WSAUnhookBlockingHook");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAGetLastError(void)
{
	GetAddress("WSAGetLastError");
	__asm JMP EAX;
}
HIJACK_CODE My_WSASetLastError(void)
{
	GetAddress("WSASetLastError");
	__asm JMP EAX;
}
HIJACK_CODE My_WSACancelBlockingCall(void)
{
	GetAddress("WSACancelBlockingCall");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAIsBlocking(void)
{
	GetAddress("WSAIsBlocking");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAStartup(void)
{
	GetAddress("WSAStartup");
	__asm JMP EAX;
}
HIJACK_CODE My_WSACleanup(void)
{
	GetAddress("WSACleanup");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAStringToAddressA(void)
{
	GetAddress("WSAStringToAddressA");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAStringToAddressW(void)
{
	GetAddress("WSAStringToAddressW");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAUnadvertiseProvider(void)
{
	GetAddress("WSAUnadvertiseProvider");
	__asm JMP EAX;
}
HIJACK_CODE My_WSAWaitForMultipleEvents(void)
{
	GetAddress("WSAWaitForMultipleEvents");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCDeinstallProvider(void)
{
	GetAddress("WSCDeinstallProvider");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCEnableNSProvider(void)
{
	GetAddress("WSCEnableNSProvider");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCEnumProtocols(void)
{
	GetAddress("WSCEnumProtocols");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCGetApplicationCategory(void)
{
	GetAddress("WSCGetApplicationCategory");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCGetProviderInfo(void)
{
	GetAddress("WSCGetProviderInfo");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCGetProviderPath(void)
{
	GetAddress("WSCGetProviderPath");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCInstallNameSpace(void)
{
	GetAddress("WSCInstallNameSpace");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCInstallNameSpaceEx(void)
{
	GetAddress("WSCInstallNameSpaceEx");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCInstallProvider(void)
{
	GetAddress("WSCInstallProvider");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCInstallProviderAndChains(void)
{
	GetAddress("WSCInstallProviderAndChains");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCSetApplicationCategory(void)
{
	GetAddress("WSCSetApplicationCategory");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCSetProviderInfo(void)
{
	GetAddress("WSCSetProviderInfo");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCUnInstallNameSpace(void)
{
	GetAddress("WSCUnInstallNameSpace");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCUpdateProvider(void)
{
	GetAddress("WSCUpdateProvider");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCWriteNameSpaceOrder(void)
{
	GetAddress("WSCWriteNameSpaceOrder");
	__asm JMP EAX;
}
HIJACK_CODE My_WSCWriteProviderOrder(void)
{
	GetAddress("WSCWriteProviderOrder");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCloseApcHelper(void)
{
	GetAddress("WahCloseApcHelper");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCloseHandleHelper(void)
{
	GetAddress("WahCloseHandleHelper");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCloseNotificationHandleHelper(void)
{
	GetAddress("WahCloseNotificationHandleHelper");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCloseSocketHandle(void)
{
	GetAddress("WahCloseSocketHandle");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCloseThread(void)
{
	GetAddress("WahCloseThread");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCompleteRequest(void)
{
	GetAddress("WahCompleteRequest");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCreateHandleContextTable(void)
{
	GetAddress("WahCreateHandleContextTable");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCreateNotificationHandle(void)
{
	GetAddress("WahCreateNotificationHandle");
	__asm JMP EAX;
}
HIJACK_CODE My_WahCreateSocketHandle(void)
{
	GetAddress("WahCreateSocketHandle");
	__asm JMP EAX;
}
HIJACK_CODE My_WahDestroyHandleContextTable(void)
{
	GetAddress("WahDestroyHandleContextTable");
	__asm JMP EAX;
}
HIJACK_CODE My_WahDisableNonIFSHandleSupport(void)
{
	GetAddress("WahDisableNonIFSHandleSupport");
	__asm JMP EAX;
}
HIJACK_CODE My_WahEnableNonIFSHandleSupport(void)
{
	GetAddress("WahEnableNonIFSHandleSupport");
	__asm JMP EAX;
}
HIJACK_CODE My_WahEnumerateHandleContexts(void)
{
	GetAddress("WahEnumerateHandleContexts");
	__asm JMP EAX;
}
HIJACK_CODE My_WahInsertHandleContext(void)
{
	GetAddress("WahInsertHandleContext");
	__asm JMP EAX;
}
HIJACK_CODE My___WSAFDIsSet(void)
{
	GetAddress("__WSAFDIsSet");
	__asm JMP EAX;
}
HIJACK_CODE My_WahNotifyAllProcesses(void)
{
	GetAddress("WahNotifyAllProcesses");
	__asm JMP EAX;
}
HIJACK_CODE My_WahOpenApcHelper(void)
{
	GetAddress("WahOpenApcHelper");
	__asm JMP EAX;
}
HIJACK_CODE My_WahOpenCurrentThread(void)
{
	GetAddress("WahOpenCurrentThread");
	__asm JMP EAX;
}
HIJACK_CODE My_WahOpenHandleHelper(void)
{
	GetAddress("WahOpenHandleHelper");
	__asm JMP EAX;
}
HIJACK_CODE My_WahOpenNotificationHandleHelper(void)
{
	GetAddress("WahOpenNotificationHandleHelper");
	__asm JMP EAX;
}
HIJACK_CODE My_WahQueueUserApc(void)
{
	GetAddress("WahQueueUserApc");
	__asm JMP EAX;
}
HIJACK_CODE My_WahReferenceContextByHandle(void)
{
	GetAddress("WahReferenceContextByHandle");
	__asm JMP EAX;
}
HIJACK_CODE My_WahRemoveHandleContext(void)
{
	GetAddress("WahRemoveHandleContext");
	__asm JMP EAX;
}
HIJACK_CODE My_WahWaitForNotification(void)
{
	GetAddress("WahWaitForNotification");
	__asm JMP EAX;
}
HIJACK_CODE My_WahWriteLSPEvent(void)
{
	GetAddress("WahWriteLSPEvent");
	__asm JMP EAX;
}
HIJACK_CODE My_freeaddrinfo(void)
{
	GetAddress("freeaddrinfo");
	__asm JMP EAX;
}
HIJACK_CODE My_getaddrinfo(void)
{
	GetAddress("getaddrinfo");
	__asm JMP EAX;
}
HIJACK_CODE My_getnameinfo(void)
{
	GetAddress("getnameinfo");
	__asm JMP EAX;
}
HIJACK_CODE My_inet_ntop(void)
{
	GetAddress("inet_ntop");
	__asm JMP EAX;
}
HIJACK_CODE My_inet_pton(void)
{
	GetAddress("inet_pton");
	__asm JMP EAX;
}
HIJACK_CODE My_WEP(void)
{
	GetAddress("WEP");
	__asm JMP EAX;
}


//定義 導出表 編號 名稱
#pragma comment(linker, "/EXPORT:accept=_My_accept,@1")
#pragma comment(linker, "/EXPORT:bind=_My_bind,@2")
#pragma comment(linker, "/EXPORT:closesocket=_My_closesocket,@3")
#pragma comment(linker, "/EXPORT:connect=_My_connect,@4")
#pragma comment(linker, "/EXPORT:getpeername=_My_getpeername,@5")
#pragma comment(linker, "/EXPORT:getsockname=_My_getsockname,@6")
#pragma comment(linker, "/EXPORT:getsockopt=_My_getsockopt,@7")
#pragma comment(linker, "/EXPORT:htonl=_My_htonl,@8")
#pragma comment(linker, "/EXPORT:htons=_My_htons,@9")
#pragma comment(linker, "/EXPORT:ioctlsocket=_My_ioctlsocket,@10")
#pragma comment(linker, "/EXPORT:inet_addr=_My_inet_addr,@11")
#pragma comment(linker, "/EXPORT:inet_ntoa=_My_inet_ntoa,@12")
#pragma comment(linker, "/EXPORT:listen=_My_listen,@13")
#pragma comment(linker, "/EXPORT:ntohl=_My_ntohl,@14")
#pragma comment(linker, "/EXPORT:ntohs=_My_ntohs,@15")
#pragma comment(linker, "/EXPORT:recv=_My_recv,@16")
#pragma comment(linker, "/EXPORT:recvfrom=_My_recvfrom,@17")
#pragma comment(linker, "/EXPORT:select=_My_select,@18")
#pragma comment(linker, "/EXPORT:send=_My_send,@19")
#pragma comment(linker, "/EXPORT:sendto=_My_sendto,@20")
#pragma comment(linker, "/EXPORT:setsockopt=_My_setsockopt,@21")
#pragma comment(linker, "/EXPORT:shutdown=_My_shutdown,@22")
#pragma comment(linker, "/EXPORT:socket=_My_socket,@23")
#pragma comment(linker, "/EXPORT:WSApSetPostRoutine=_My_WSApSetPostRoutine,@24")
#pragma comment(linker, "/EXPORT:FreeAddrInfoEx=_My_FreeAddrInfoEx,@25")
#pragma comment(linker, "/EXPORT:FreeAddrInfoExW=_My_FreeAddrInfoExW,@26")
#pragma comment(linker, "/EXPORT:FreeAddrInfoW=_My_FreeAddrInfoW,@27")
#pragma comment(linker, "/EXPORT:GetAddrInfoExA=_My_GetAddrInfoExA,@28")
#pragma comment(linker, "/EXPORT:GetAddrInfoExW=_My_GetAddrInfoExW,@29")
#pragma comment(linker, "/EXPORT:GetAddrInfoW=_My_GetAddrInfoW,@30")
#pragma comment(linker, "/EXPORT:GetNameInfoW=_My_GetNameInfoW,@31")
#pragma comment(linker, "/EXPORT:InetNtopW=_My_InetNtopW,@32")
#pragma comment(linker, "/EXPORT:InetPtonW=_My_InetPtonW,@33")
#pragma comment(linker, "/EXPORT:SetAddrInfoExA=_My_SetAddrInfoExA,@34")
#pragma comment(linker, "/EXPORT:SetAddrInfoExW=_My_SetAddrInfoExW,@35")
#pragma comment(linker, "/EXPORT:WPUCompleteOverlappedRequest=_My_WPUCompleteOverlappedRequest,@36")
#pragma comment(linker, "/EXPORT:WSAAccept=_My_WSAAccept,@37")
#pragma comment(linker, "/EXPORT:WSAAddressToStringA=_My_WSAAddressToStringA,@38")
#pragma comment(linker, "/EXPORT:WSAAddressToStringW=_My_WSAAddressToStringW,@39")
#pragma comment(linker, "/EXPORT:WSAAdvertiseProvider=_My_WSAAdvertiseProvider,@40")
#pragma comment(linker, "/EXPORT:WSACloseEvent=_My_WSACloseEvent,@41")
#pragma comment(linker, "/EXPORT:WSAConnect=_My_WSAConnect,@42")
#pragma comment(linker, "/EXPORT:WSAConnectByList=_My_WSAConnectByList,@43")
#pragma comment(linker, "/EXPORT:WSAConnectByNameA=_My_WSAConnectByNameA,@44")
#pragma comment(linker, "/EXPORT:WSAConnectByNameW=_My_WSAConnectByNameW,@45")
#pragma comment(linker, "/EXPORT:WSACreateEvent=_My_WSACreateEvent,@46")
#pragma comment(linker, "/EXPORT:WSADuplicateSocketA=_My_WSADuplicateSocketA,@47")
#pragma comment(linker, "/EXPORT:WSADuplicateSocketW=_My_WSADuplicateSocketW,@48")
#pragma comment(linker, "/EXPORT:WSAEnumNameSpaceProvidersA=_My_WSAEnumNameSpaceProvidersA,@49")
#pragma comment(linker, "/EXPORT:WSAEnumNameSpaceProvidersExA=_My_WSAEnumNameSpaceProvidersExA,@50")
#pragma comment(linker, "/EXPORT:gethostbyaddr=_My_gethostbyaddr,@51")
#pragma comment(linker, "/EXPORT:gethostbyname=_My_gethostbyname,@52")
#pragma comment(linker, "/EXPORT:getprotobyname=_My_getprotobyname,@53")
#pragma comment(linker, "/EXPORT:getprotobynumber=_My_getprotobynumber,@54")
#pragma comment(linker, "/EXPORT:getservbyname=_My_getservbyname,@55")
#pragma comment(linker, "/EXPORT:getservbyport=_My_getservbyport,@56")
#pragma comment(linker, "/EXPORT:gethostname=_My_gethostname,@57")
#pragma comment(linker, "/EXPORT:WSAEnumNameSpaceProvidersExW=_My_WSAEnumNameSpaceProvidersExW,@58")
#pragma comment(linker, "/EXPORT:WSAEnumNameSpaceProvidersW=_My_WSAEnumNameSpaceProvidersW,@59")
#pragma comment(linker, "/EXPORT:WSAEnumNetworkEvents=_My_WSAEnumNetworkEvents,@60")
#pragma comment(linker, "/EXPORT:WSAEnumProtocolsA=_My_WSAEnumProtocolsA,@61")
#pragma comment(linker, "/EXPORT:WSAEnumProtocolsW=_My_WSAEnumProtocolsW,@62")
#pragma comment(linker, "/EXPORT:WSAEventSelect=_My_WSAEventSelect,@63")
#pragma comment(linker, "/EXPORT:WSAGetOverlappedResult=_My_WSAGetOverlappedResult,@64")
#pragma comment(linker, "/EXPORT:WSAGetQOSByName=_My_WSAGetQOSByName,@65")
#pragma comment(linker, "/EXPORT:WSAGetServiceClassInfoA=_My_WSAGetServiceClassInfoA,@66")
#pragma comment(linker, "/EXPORT:WSAGetServiceClassInfoW=_My_WSAGetServiceClassInfoW,@67")
#pragma comment(linker, "/EXPORT:WSAGetServiceClassNameByClassIdA=_My_WSAGetServiceClassNameByClassIdA,@68")
#pragma comment(linker, "/EXPORT:WSAGetServiceClassNameByClassIdW=_My_WSAGetServiceClassNameByClassIdW,@69")
#pragma comment(linker, "/EXPORT:WSAHtonl=_My_WSAHtonl,@70")
#pragma comment(linker, "/EXPORT:WSAHtons=_My_WSAHtons,@71")
#pragma comment(linker, "/EXPORT:WSAInstallServiceClassA=_My_WSAInstallServiceClassA,@72")
#pragma comment(linker, "/EXPORT:WSAInstallServiceClassW=_My_WSAInstallServiceClassW,@73")
#pragma comment(linker, "/EXPORT:WSAIoctl=_My_WSAIoctl,@74")
#pragma comment(linker, "/EXPORT:WSAJoinLeaf=_My_WSAJoinLeaf,@75")
#pragma comment(linker, "/EXPORT:WSALookupServiceBeginA=_My_WSALookupServiceBeginA,@76")
#pragma comment(linker, "/EXPORT:WSALookupServiceBeginW=_My_WSALookupServiceBeginW,@77")
#pragma comment(linker, "/EXPORT:WSALookupServiceEnd=_My_WSALookupServiceEnd,@78")
#pragma comment(linker, "/EXPORT:WSALookupServiceNextA=_My_WSALookupServiceNextA,@79")
#pragma comment(linker, "/EXPORT:WSALookupServiceNextW=_My_WSALookupServiceNextW,@80")
#pragma comment(linker, "/EXPORT:WSANSPIoctl=_My_WSANSPIoctl,@81")
#pragma comment(linker, "/EXPORT:WSANtohl=_My_WSANtohl,@82")
#pragma comment(linker, "/EXPORT:WSANtohs=_My_WSANtohs,@83")
#pragma comment(linker, "/EXPORT:WSAPoll=_My_WSAPoll,@84")
#pragma comment(linker, "/EXPORT:WSAProviderCompleteAsyncCall=_My_WSAProviderCompleteAsyncCall,@85")
#pragma comment(linker, "/EXPORT:WSAProviderConfigChange=_My_WSAProviderConfigChange,@86")
#pragma comment(linker, "/EXPORT:WSARecv=_My_WSARecv,@87")
#pragma comment(linker, "/EXPORT:WSARecvDisconnect=_My_WSARecvDisconnect,@88")
#pragma comment(linker, "/EXPORT:WSARecvFrom=_My_WSARecvFrom,@89")
#pragma comment(linker, "/EXPORT:WSARemoveServiceClass=_My_WSARemoveServiceClass,@90")
#pragma comment(linker, "/EXPORT:WSAResetEvent=_My_WSAResetEvent,@91")
#pragma comment(linker, "/EXPORT:WSASend=_My_WSASend,@92")
#pragma comment(linker, "/EXPORT:WSASendDisconnect=_My_WSASendDisconnect,@93")
#pragma comment(linker, "/EXPORT:WSASendMsg=_My_WSASendMsg,@94")
#pragma comment(linker, "/EXPORT:WSASendTo=_My_WSASendTo,@95")
#pragma comment(linker, "/EXPORT:WSASetEvent=_My_WSASetEvent,@96")
#pragma comment(linker, "/EXPORT:WSASetServiceA=_My_WSASetServiceA,@97")
#pragma comment(linker, "/EXPORT:WSASetServiceW=_My_WSASetServiceW,@98")
#pragma comment(linker, "/EXPORT:WSASocketA=_My_WSASocketA,@99")
#pragma comment(linker, "/EXPORT:WSASocketW=_My_WSASocketW,@100")
#pragma comment(linker, "/EXPORT:WSAAsyncSelect=_My_WSAAsyncSelect,@101")
#pragma comment(linker, "/EXPORT:WSAAsyncGetHostByAddr=_My_WSAAsyncGetHostByAddr,@102")
#pragma comment(linker, "/EXPORT:WSAAsyncGetHostByName=_My_WSAAsyncGetHostByName,@103")
#pragma comment(linker, "/EXPORT:WSAAsyncGetProtoByNumber=_My_WSAAsyncGetProtoByNumber,@104")
#pragma comment(linker, "/EXPORT:WSAAsyncGetProtoByName=_My_WSAAsyncGetProtoByName,@105")
#pragma comment(linker, "/EXPORT:WSAAsyncGetServByPort=_My_WSAAsyncGetServByPort,@106")
#pragma comment(linker, "/EXPORT:WSAAsyncGetServByName=_My_WSAAsyncGetServByName,@107")
#pragma comment(linker, "/EXPORT:WSACancelAsyncRequest=_My_WSACancelAsyncRequest,@108")
#pragma comment(linker, "/EXPORT:WSASetBlockingHook=_My_WSASetBlockingHook,@109")
#pragma comment(linker, "/EXPORT:WSAUnhookBlockingHook=_My_WSAUnhookBlockingHook,@110")
#pragma comment(linker, "/EXPORT:WSAGetLastError=_My_WSAGetLastError,@111")
#pragma comment(linker, "/EXPORT:WSASetLastError=_My_WSASetLastError,@112")
#pragma comment(linker, "/EXPORT:WSACancelBlockingCall=_My_WSACancelBlockingCall,@113")
#pragma comment(linker, "/EXPORT:WSAIsBlocking=_My_WSAIsBlocking,@114")
#pragma comment(linker, "/EXPORT:WSAStartup=_My_WSAStartup,@115")
#pragma comment(linker, "/EXPORT:WSACleanup=_My_WSACleanup,@116")
#pragma comment(linker, "/EXPORT:WSAStringToAddressA=_My_WSAStringToAddressA,@117")
#pragma comment(linker, "/EXPORT:WSAStringToAddressW=_My_WSAStringToAddressW,@118")
#pragma comment(linker, "/EXPORT:WSAUnadvertiseProvider=_My_WSAUnadvertiseProvider,@119")
#pragma comment(linker, "/EXPORT:WSAWaitForMultipleEvents=_My_WSAWaitForMultipleEvents,@120")
#pragma comment(linker, "/EXPORT:WSCDeinstallProvider=_My_WSCDeinstallProvider,@121")
#pragma comment(linker, "/EXPORT:WSCEnableNSProvider=_My_WSCEnableNSProvider,@122")
#pragma comment(linker, "/EXPORT:WSCEnumProtocols=_My_WSCEnumProtocols,@123")
#pragma comment(linker, "/EXPORT:WSCGetApplicationCategory=_My_WSCGetApplicationCategory,@124")
#pragma comment(linker, "/EXPORT:WSCGetProviderInfo=_My_WSCGetProviderInfo,@125")
#pragma comment(linker, "/EXPORT:WSCGetProviderPath=_My_WSCGetProviderPath,@126")
#pragma comment(linker, "/EXPORT:WSCInstallNameSpace=_My_WSCInstallNameSpace,@127")
#pragma comment(linker, "/EXPORT:WSCInstallNameSpaceEx=_My_WSCInstallNameSpaceEx,@128")
#pragma comment(linker, "/EXPORT:WSCInstallProvider=_My_WSCInstallProvider,@129")
#pragma comment(linker, "/EXPORT:WSCInstallProviderAndChains=_My_WSCInstallProviderAndChains,@130")
#pragma comment(linker, "/EXPORT:WSCSetApplicationCategory=_My_WSCSetApplicationCategory,@131")
#pragma comment(linker, "/EXPORT:WSCSetProviderInfo=_My_WSCSetProviderInfo,@132")
#pragma comment(linker, "/EXPORT:WSCUnInstallNameSpace=_My_WSCUnInstallNameSpace,@133")
#pragma comment(linker, "/EXPORT:WSCUpdateProvider=_My_WSCUpdateProvider,@134")
#pragma comment(linker, "/EXPORT:WSCWriteNameSpaceOrder=_My_WSCWriteNameSpaceOrder,@135")
#pragma comment(linker, "/EXPORT:WSCWriteProviderOrder=_My_WSCWriteProviderOrder,@136")
#pragma comment(linker, "/EXPORT:WahCloseApcHelper=_My_WahCloseApcHelper,@137")
#pragma comment(linker, "/EXPORT:WahCloseHandleHelper=_My_WahCloseHandleHelper,@138")
#pragma comment(linker, "/EXPORT:WahCloseNotificationHandleHelper=_My_WahCloseNotificationHandleHelper,@139")
#pragma comment(linker, "/EXPORT:WahCloseSocketHandle=_My_WahCloseSocketHandle,@140")
#pragma comment(linker, "/EXPORT:WahCloseThread=_My_WahCloseThread,@141")
#pragma comment(linker, "/EXPORT:WahCompleteRequest=_My_WahCompleteRequest,@142")
#pragma comment(linker, "/EXPORT:WahCreateHandleContextTable=_My_WahCreateHandleContextTable,@143")
#pragma comment(linker, "/EXPORT:WahCreateNotificationHandle=_My_WahCreateNotificationHandle,@144")
#pragma comment(linker, "/EXPORT:WahCreateSocketHandle=_My_WahCreateSocketHandle,@145")
#pragma comment(linker, "/EXPORT:WahDestroyHandleContextTable=_My_WahDestroyHandleContextTable,@146")
#pragma comment(linker, "/EXPORT:WahDisableNonIFSHandleSupport=_My_WahDisableNonIFSHandleSupport,@147")
#pragma comment(linker, "/EXPORT:WahEnableNonIFSHandleSupport=_My_WahEnableNonIFSHandleSupport,@148")
#pragma comment(linker, "/EXPORT:WahEnumerateHandleContexts=_My_WahEnumerateHandleContexts,@149")
#pragma comment(linker, "/EXPORT:WahInsertHandleContext=_My_WahInsertHandleContext,@150")
#pragma comment(linker, "/EXPORT:__WSAFDIsSet=_My___WSAFDIsSet,@151")
#pragma comment(linker, "/EXPORT:WahNotifyAllProcesses=_My_WahNotifyAllProcesses,@152")
#pragma comment(linker, "/EXPORT:WahOpenApcHelper=_My_WahOpenApcHelper,@153")
#pragma comment(linker, "/EXPORT:WahOpenCurrentThread=_My_WahOpenCurrentThread,@154")
#pragma comment(linker, "/EXPORT:WahOpenHandleHelper=_My_WahOpenHandleHelper,@155")
#pragma comment(linker, "/EXPORT:WahOpenNotificationHandleHelper=_My_WahOpenNotificationHandleHelper,@156")
#pragma comment(linker, "/EXPORT:WahQueueUserApc=_My_WahQueueUserApc,@157")
#pragma comment(linker, "/EXPORT:WahReferenceContextByHandle=_My_WahReferenceContextByHandle,@158")
#pragma comment(linker, "/EXPORT:WahRemoveHandleContext=_My_WahRemoveHandleContext,@159")
#pragma comment(linker, "/EXPORT:WahWaitForNotification=_My_WahWaitForNotification,@160")
#pragma comment(linker, "/EXPORT:WahWriteLSPEvent=_My_WahWriteLSPEvent,@161")
#pragma comment(linker, "/EXPORT:freeaddrinfo=_My_freeaddrinfo,@162")
#pragma comment(linker, "/EXPORT:getaddrinfo=_My_getaddrinfo,@163")
#pragma comment(linker, "/EXPORT:getnameinfo=_My_getnameinfo,@164")
#pragma comment(linker, "/EXPORT:inet_ntop=_My_inet_ntop,@165")
#pragma comment(linker, "/EXPORT:inet_pton=_My_inet_pton,@166")
#pragma comment(linker, "/EXPORT:WEP=_My_WEP,@500")


