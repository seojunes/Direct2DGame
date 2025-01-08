#include "TWorld.h"
#include "TScene.h"

void TWorld::AddCollisionExecute(TObject* pOwner, CollisionFunction fun)
{
	pOwner->m_iCollisionID = ++m_iExecuteCollisionID;
	m_CollisionList.insert(std::make_pair(pOwner->m_iCollisionID, pOwner));
	m_fnCollisionExecute.insert(std::make_pair(pOwner->m_iCollisionID, fun));
}
void TWorld::DeleteCollisionExecute(TObject* pOwner)
{
	auto iter = m_CollisionList.find(pOwner->m_iCollisionID);
	if (iter != m_CollisionList.end())
	{
		m_CollisionList.erase(iter);
	}
	auto iterFun = m_fnCollisionExecute.find(pOwner->m_iCollisionID);
	if (iterFun != m_fnCollisionExecute.end())
	{
		m_fnCollisionExecute.erase(iterFun);
	}
}
void TWorld::AddSelectExecute(TObject* pOwner, CollisionFunction fun)
{
	pOwner->m_iSelectID = ++m_iExecuteSelectID;
	m_SelectList.insert(std::make_pair(pOwner->m_iSelectID, pOwner));
	m_fnSelectExecute.insert(std::make_pair(pOwner->m_iSelectID, fun));
}
void TWorld::DeleteSelectExecute(TObject* pOwner)
{
	auto iter = m_SelectList.find(pOwner->m_iSelectID);
	if (iter != m_SelectList.end())
	{
		m_SelectList.erase(iter);
	}
	auto iterFun = m_fnSelectExecute.find(pOwner->m_iSelectID);
	if (iterFun != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(iterFun);
	}
}
void   TWorld::Frame()
{
	for (auto src : m_CollisionList)
	{
		TObject* pSrcObj = src.second;
		if (pSrcObj->m_bDead) continue;
		if (pSrcObj->m_iCollisionType == TCollisionType::T_Ignore)
		{
			continue;
		}
		for (auto dest : m_CollisionList)
		{
			if (src == dest) continue;
			TObject* pDestObj = dest.second;
			if (pDestObj->m_bDead) continue;
			if (pSrcObj->m_iCollisionType == TCollisionType::T_Ignore)
			{
				continue;
			}
			if (TCollision::CheckRectToRect(pSrcObj->m_rtScreen, pDestObj->m_rtScreen))
			{
				auto iter = m_fnCollisionExecute.find(pSrcObj->m_iCollisionID);
				if (iter != m_fnCollisionExecute.end())
				{
					THitResult ret;
					CollisionFunction call = iter->second;
					call(pDestObj, ret);
				}
			}
		}
	}


	for (auto src : m_SelectList)
	{
		TObject* pSrcObj = src.second;
		if (pSrcObj->m_bDead) continue;
		if (pSrcObj->m_iCollisionType == TCollisionType::T_Ignore)
		{
			continue;
		}

		if (TCollision::CheckRectToPoint(pSrcObj->m_rtScreen, g_ptMouse))
		{
			auto iter = m_fnSelectExecute.find(pSrcObj->m_iSelectID);
			if (iter != m_fnSelectExecute.end())
			{
				THitResult ret;
				//ret.iState = TSelectState::T_DEFAULT;
				ret.iState = TSelectState::T_HOVER;
				if (g_GameKey.dwLeftClick == KEY_PUSH ||g_GameKey.dwLeftClick == KEY_HOLD)
				{
					ret.iState = TSelectState::T_ACTIVE;
				}
				if (g_GameKey.dwLeftClick == KEY_UP)
				{
					ret.iState = TSelectState::T_SELECTED;
				}
				CollisionFunction call = iter->second;
				call(nullptr, ret);
			}
		}
		else
		{
			if (pSrcObj->m_iSelectState != TSelectState::T_DEFAULT)
			{
				auto iter = m_fnSelectExecute.find(pSrcObj->m_iSelectID);
				if (iter != m_fnSelectExecute.end())
				{
					THitResult ret;
					ret.iState = TSelectState::T_DEFAULT;
					CollisionFunction call = iter->second;
					call(nullptr, ret);
				}
			}
		}
	}
}
void   TWorld::Release()
{
	for (auto src : m_CollisionList)
	{
		TObject* pSrcObj = src.second;
		pSrcObj->Release();
	}
	for (auto src : m_SelectList)
	{
		TObject* pSrcObj = src.second;
		pSrcObj->Release();
	}
	m_CollisionList.clear();
	m_SelectList.clear();
}

TWorld::TWorld(TScene* pScene) : m_pScene(pScene)
{

}