#include "TWorld.h"
void TWorld::AddCollisionExecute(
	TObject* pOwner,
	CollisionFunction fun)
{
	pOwner->m_iCollisionID = ++m_iExecuteCollisionID;
	m_ObjectList.insert(std::make_pair(
		pOwner->m_iCollisionID,
		pOwner));
	m_fnCollisionExecute.insert(std::make_pair(
		pOwner->m_iCollisionID,
		fun));

	/*auto iter = m_fnCollisionExecute.find(
		pOwner->m_iCollisionID);
	if (iter != m_fnCollisionExecute.end())
	{
		CollisionFunction call = iter->second;
		call(nullptr, 999);
	}*/
}
void TWorld::DeleteCollisionExecute(TObject* pOwner)
{
	auto iter = m_ObjectList.find(pOwner->m_iCollisionID);
	if (iter != m_ObjectList.end())
	{
		m_ObjectList.erase(iter);
	}
	auto iterFun = m_fnCollisionExecute.find(pOwner->m_iCollisionID);
	if (iterFun != m_fnCollisionExecute.end())
	{
		m_fnCollisionExecute.erase(iterFun);
	}
}
void   TWorld::Frame()
{
	// Ãæµ¹º¼·ý(TRect, TSphere)
	// ¼±ÅÃº¼·ý(Mouse)
	for (auto src : m_ObjectList)
	{
		TObject* pSrcObj = src.second;
		if (pSrcObj->m_bDead) continue;
		if (pSrcObj->m_iCollisionType ==
			TCollisionType::T_Ignore)
		{
			continue;
		}
		for (auto dest : m_ObjectList)
		{
			if (src == dest) continue;
			TObject* pDestObj = dest.second;
			if (pDestObj->m_bDead) continue;
			if (pSrcObj->m_iCollisionType ==
				TCollisionType::T_Ignore)
			{
				continue;
			}
			if (TCollision::CheckRectToRect(
				pSrcObj->m_rtScreen,
				pDestObj->m_rtScreen))
			{
				auto iter = m_fnCollisionExecute.find(
					pSrcObj->m_iCollisionID);
				if (iter != m_fnCollisionExecute.end())
				{
					THitResult ret;
					CollisionFunction call = iter->second;
					call(pDestObj, ret);
				}
			}
		}
	}
}