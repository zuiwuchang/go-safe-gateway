#pragma once
class singleton_ex
	:public kg::utils::singleton<singleton_ex>
{
public:
	singleton_ex(void);
	~singleton_ex(void);
	void init(WSPPROC_TABLE* table);
private:
	//原服務實現
	WSPPROC_TABLE* _table;

	//AcceptEx 同步
	boost::mutex _mutexAcceptEx;
	//AcceptEx 地址
	LPFN_ACCEPTEX _AcceptEx;

	//
	boost::mutex _mutexS;
	boost::unordered_map<SOCKET,bool/*是否已連接成功*/> _sockets;
public:
	//初始化 AcceptEx 地址
	int AcceptEx(  __in   SOCKET s,
  __in   DWORD dwIoControlCode,
  __in   LPVOID lpvInBuffer,
  __in   DWORD cbInBuffer,
  __out  LPVOID lpvOutBuffer,
  __in   DWORD cbOutBuffer,
  __out  LPDWORD lpcbBytesReturned,
  __in   LPWSAOVERLAPPED lpOverlapped,
  __in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
  __in   LPWSATHREADID lpThreadId,
  __out  LPINT lpErrno);

	//返回 AcceptEx 地址
	inline LPFN_ACCEPTEX AcceptEx()const
	{
		return _AcceptEx;
	}

	//增加一個 AcceptEx 連接記錄
	inline void insert(SOCKET s)
	{
		try
		{
			boost::mutex::scoped_lock(_mutexS);
			_sockets[s] = false;
		}
		catch(const std::bad_alloc&)
		{
		}
	}
	//刪除 AcceptEx 記錄
	inline void erase(SOCKET s)
	{
		boost::mutex::scoped_lock(_mutexS);
		BOOST_AUTO(find,_sockets.find(s));
		if(_sockets.end() != find)
		{
			_sockets.erase(find);
		}
	}

	//返回 是否 安全
	bool request_safe(SOCKET s);
};

