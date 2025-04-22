#include "TRadderObj.h"
#include "TWorld.h"
void    TRadderObj::HitOverlap(Object* pObj, THitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();

	if (OtherType == ObjectType::Hero)
	{
		auto pHero = dynamic_cast<THeroObj*>(pObj);
		//pHero->m_fGravity = 0;
		pHero->m_fVerticalSpeed = 0;
		//pHero->GetGroundH(m_rtScreen.v1.y);
		pHero->m_CurrentState = HeroState::Radder;
		pHero->m_iJumpingCount = 0;
	}

}