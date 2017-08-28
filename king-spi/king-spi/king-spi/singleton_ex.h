#pragma once
class singleton_ex
	:public kg::utils::singleton<singleton_ex>
{
public:
	singleton_ex(void);
	~singleton_ex(void);
	void init(WSPPROC_TABLE* table);
private:
	//ԭ���Ռ��F
	WSPPROC_TABLE* _table;

	//AcceptEx ͬ��
	boost::mutex _mutexAcceptEx;
	//AcceptEx ��ַ
	LPFN_ACCEPTEX _AcceptEx;

	//
	boost::mutex _mutexS;
	boost::unordered_map<SOCKET,bool/*�Ƿ����B�ӳɹ�*/> _sockets;
public:
	//��ʼ�� AcceptEx ��ַ
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

	//���� AcceptEx ��ַ
	inline LPFN_ACCEPTEX AcceptEx()const
	{
		return _AcceptEx;
	}

	//����һ�� AcceptEx �B��ӛ�
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
	//�h�� AcceptEx ӛ�
	inline void erase(SOCKET s)
	{
		boost::mutex::scoped_lock(_mutexS);
		BOOST_AUTO(find,_sockets.find(s));
		if(_sockets.end() != find)
		{
			_sockets.erase(find);
		}
	}

	//���� �Ƿ� ��ȫ
	bool request_safe(SOCKET s);
};

