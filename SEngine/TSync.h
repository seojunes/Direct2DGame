#pragma once
#include <windows.h>
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

