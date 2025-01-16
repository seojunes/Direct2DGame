#include "TUiHpBar.h"

void TUiHpBar::Frame()
{
	if(m_eBarOwner == HPBAR_OWNER::BAROWNER_HERO)
	{
		if (m_pHero == nullptr) return;

		m_iHP = m_pHero->m_HeroHP;
		m_fHpRatio = (float)m_iHP / m_iMaxHp;

		TVector2 ratio = { m_fHpRatio * m_vInitialScale.x, m_vInitialScale.y };
		SetScale(ratio);
		m_vPos.x = 331.684998f;
		SetPosition({ m_vPos.x - (m_vInitialScale.x - ratio.x) , m_vPos.y });  //hp바 좌측고정.
	}
	else
	{
		if (m_pBoss == nullptr) return;

		m_iBHP = m_pBoss->m_HP;
		m_fBHpRatio = (float)m_iBHP / m_iBMaxHp;

		TVector2 ratio = { m_fBHpRatio * m_vBInitialScale.x, m_vBInitialScale.y };
		SetScale(ratio);
		m_vPos.x = 679.125f;
		SetPosition({ m_vPos.x - (m_vBInitialScale.x - ratio.x) , m_vPos.y });  //hp바 좌측고정.
	}
	
	
}


void TUiHpBar::SetHeroHp(INT hp)
{
	m_iHP = hp;
	m_iMaxHp = hp;
}
void TUiHpBar::SetBossHp(INT hp)
{
	m_iBHP = hp;
	m_iBMaxHp = hp;
}
