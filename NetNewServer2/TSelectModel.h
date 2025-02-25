#pragma once
#include "THost.h"
class TNetwork;
class TSelectModel
{
public:
	SOCKET		m_Sock;
	TNetwork*   m_pNet=nullptr;
public:
	virtual bool  Init(TNetwork* pNet) = 0;
	virtual bool  Run() = 0;
};

class TAsyncSelect : public TSelectModel
{
public:
	virtual bool  Init(TNetwork* pNet) { return true; }
	virtual bool  Run() { return true; }
};

class TStdSelect : public TSelectModel
{
public:
	virtual bool  Init(TNetwork* pNet) { return true; }
	virtual bool  Run() { return true; }
};

class TEventSelect : public TSelectModel
{
	std::vector<WSAEVENT>  m_EventArray;
	std::vector<SOCKET>    m_SockArray;
	std::vector<bool>      m_bDisconnectArray;
public:
	THost* GetHost(SOCKET sock);
	virtual bool  Init(TNetwork* pNet);
	virtual bool  Run();
	void    Rebuild();
};
