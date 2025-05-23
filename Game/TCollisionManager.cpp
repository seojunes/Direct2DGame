#include "TCollisionManager.h"
#include "TWorld.h"
void    TCollisionManager::HitOverlap(Object* pObj, HitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
 	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();
	
	if (OtherType == ObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile)
		{
			pMissile->m_bDead = true;
		}
	}
	
}




//vector<TVector2> TCollisionManager::MakeSBox()
//{
//	rectSList.emplace_back(m_vStart);
//	return rectSList;
//}
//vector<TVector2> TCollisionManager::MakeEBox() 
//{
//	rectEList.emplace_back(m_vEnd);
//	return rectEList;
//}

