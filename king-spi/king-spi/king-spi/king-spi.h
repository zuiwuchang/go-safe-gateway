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
);

BOOL PASCAL FAR ExtAcceptEx(
    __in SOCKET sListenSocket,
    __in SOCKET sAcceptSocket,
    __in PVOID lpOutputBuffer,
    __in DWORD dwReceiveDataLength,
    __in DWORD dwLocalAddressLength,
    __in DWORD dwRemoteAddressLength,
    __out LPDWORD lpdwBytesReceived,
    __inout LPOVERLAPPED lpOverlapped
);