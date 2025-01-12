#include "THpBar.h"

void THpBar::Frame()
{
	m_iHP = m_pNpc->m_HP;
	m_fHpRatio = (float)m_iHP / m_iMaxHp;
	TVector2 ratio = { m_fHpRatio * m_vInitialScale.x, m_vInitialScale.y };
	SetScale(ratio);
	
	m_vPos.x = m_pNpc->m_vPos.x;
	SetPosition({ m_vPos.x- (m_vInitialScale.x - ratio.x) , m_vPos.y});  //hp바 좌측고정.
	if (m_iHP == 0)
	{
		m_bDead = true;
	}
}

void THpBar::Render()
{
	/*if (m_HPSTATE == HPSTATE::STATE_IDEL)
	{
		return;
	}*/

	TObject2D::Render();
}