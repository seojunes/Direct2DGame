#include "TCollisionManager.h"
#include "TWorld.h"
void    TCollisionManager::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
 	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();
	
	if (OtherType == TObjectType::Projectile)
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

