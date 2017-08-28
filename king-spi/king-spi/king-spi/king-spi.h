#pragma once

int WSPAPI WSPStartup(
	WORD				wVersionRequested,
	LPWSPDATA			lpWSPData,
	LPWSAPROTOCOL_INFOW	lpProtocolInfo,
	WSPUPCALLTABLE		upcallTable,
	LPWSPPROC_TABLE		lpProcTable
);


SOCKET WSPAPI WSPAccept(
    __in SOCKET s,
    __out_bcount_part_opt(*addrlen, *addrlen) struct sockaddr FAR * addr,
    __inout_opt LPINT addrlen,
    __in_opt LPCONDITIONPROC lpfnCondition,
    __in_opt DWORD_PTR dwCallbackData,
    __out LPINT lpErrno
);
int WSPAPI WSPCloseSocket(
  __in   SOCKET s,
  __out  LPINT lpErrno
);
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
);