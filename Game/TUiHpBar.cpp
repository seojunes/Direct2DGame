#include "TUiHpBar.h"

void TUiHpBar::Frame()
{
	if (m_pHero == nullptr) return;


	m_iHP = m_pHero->m_HP;
	m_fHpRatio = (float)m_iHP / m_iMaxHp;

	TVector2 ratio = { m_fHpRatio * m_vInitialScale.x, m_vInitialScale.y };
	SetScale(ratio);
	
}

