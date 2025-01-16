#include "TDropObj.h"
#include "THeroObj.h"
void    TDropObj::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();

	if (OtherType == TObjectType::Wall)
	{
		m_bDead = true;
	}
	else if (OtherType == TObjectType::Hero)
	{
		auto pHero = dynamic_cast<THeroObj*>(pObj);
		pHero->TakeDamage(30);
		m_bDead = true;
	}

}
void TDropObj::Frame()
{
	m_vDir = { 0.0f, 1.0f };
	m_fSpeed = 250.0f;
	m_vPos = m_vPos + m_vDir * g_fSPF * m_fSpeed;
	SetPosition(m_vPos);
}