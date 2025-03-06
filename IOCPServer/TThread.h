#pragma once
#include "TSync.h"
#include <process.h>
class TThread : public TLockObj
{
public:
	unsigned int m_hThread;
	unsigned int m_ithreadID;
	bool		 m_bRun;
public:
	void		 CreateThread();
	static unsigned int WINAPI HandleRunner(LPVOID arg);
	virtual bool RunThread() { return true; }
public:
	TThread();
	virtual ~TThread();
};

