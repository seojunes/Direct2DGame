#include "TRadderObj.h"
#include "TWorld.h"
void    TRadderObj::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();

	if (OtherType == TObjectType::Hero)
	{
		auto pHero = dynamic_cast<THeroObj*>(pObj);
		//pHero->m_fGravity = 0;
		pHero->m_fVerticalSpeed = 0;
		//pHero->GetGroundH(m_rtScreen.v1.y);
		pHero->m_CurrentState = HeroState::Radder;
		pHero->m_iJumpingCount = 0;
	}

}