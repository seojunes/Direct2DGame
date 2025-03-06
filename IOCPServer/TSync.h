#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <list>
#include <vector>
#include "TProtocol.h"
#include "TObjectPool.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

class TLockObj
{
public:
	CRITICAL_SECTION  m_cs;
	TLockObj();
	~TLockObj();
};

class TSync
{
private:
	TLockObj* m_pObj = nullptr;
public:	
	TSync(TLockObj* pObj);
	virtual ~TSync();
};

