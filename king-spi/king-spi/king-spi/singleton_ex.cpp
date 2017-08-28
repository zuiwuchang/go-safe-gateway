#include "StdAfx.h"
#include "singleton_ex.h"
#include "singleton_filter.h"


singleton_ex::singleton_ex(void)
	:_AcceptEx(NULL)
{
}


singleton_ex::~singleton_ex(void)
{
}
void singleton_ex::init(WSPPROC_TABLE* table)
{
	_table = table;
}
int singleton_ex::AcceptEx(  __in   SOCKET s,
  __in   DWORD dwIoControlCode,
  __in   LPVOID lpvInBuffer,
  __in   DWORD cbInBuffer,
  __out  LPVOID lpvOutBuffer,
  __in   DWORD cbOutBuffer,
  __out  LPDWORD lpcbBytesReturned,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in   LPWSATHREADID lpThreadId,
  __out  LPINT lpErrno)
{
	if(_AcceptEx)
	{
		return NO_ERROR;
	}

	boost::mutex::scoped_lock(_mutexAcceptEx);
	if(_AcceptEx)
	{
		return NO_ERROR;
	}

	int rs = _table->lpWSPIoctl(s
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
	if(NO_ERROR == rs)
	{
		_AcceptEx = *((LPFN_ACCEPTEX *)lpvOutBuffer);
	}
	return rs;
}
bool singleton_ex::request_safe(SOCKET s)
{
	boost::mutex::scoped_lock(_mutexS);
	
	//²éÔƒ AcceptEx Ó›ä›
	BOOST_AUTO(find,_sockets.find(s));
	
	//accept recv
	if(_sockets.end() == find)
	{
		return true;
	}
	
	//AcceptEx recv
	if(find->second)
	{
		return true;
	}
	//AcceptEx success
	find->second = true;
	return singleton_filter::get_mutable_instance().request_safe(s);
}