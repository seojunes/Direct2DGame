#include "TSync.h"

TLockObj::TLockObj()
{
	::InitializeCriticalSection(&m_cs);
}
TLockObj::~TLockObj()
{
	::DeleteCriticalSection(&m_cs);
}

TSync::TSync(TLockObj* pObj) : m_pObj(pObj)
{
	if (pObj == nullptr) return;
	::EnterCriticalSection(&pObj->m_cs);
}

TSync::~TSync()
{
	if (m_pObj == nullptr) return;
	::LeaveCriticalSection(&m_pObj->m_cs);
}