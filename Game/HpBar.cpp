#include "HpBar.h"

void HpBar::Frame()
{
	if (m_pNpc == nullptr) return;

	// 현재 HP 가져오기
	INT currentHP = m_pNpc->m_HP;

	if (currentHP != m_iPreviousHP)
	{
		if (m_iPreviousHP != 0)
		{
			m_fShowTime = m_fMaxShowTime; // 타이머 초기화
		}
		m_iPreviousHP = currentHP;   // 이전 HP 갱신
	}

	// 타이머 감소
	if (m_fShowTime > 0.0f)
	{
		m_fShowTime -= g_fSPF; 
	}

	m_iHP = m_pNpc->m_HP;
	m_fHpRatio = (float)m_iHP / m_iMaxHp;
	
	TVector2 ratio = { m_fHpRatio * m_vInitialScale.x, m_vInitialScale.y };
	SetScale(ratio);
	
	m_vPos.x = m_pNpc->m_vPos.x;
	m_vPos.y = m_pNpc->m_rtScreen.v1.y - 30.0f;
	SetPosition({ m_vPos.x- (m_vInitialScale.x - ratio.x) , m_vPos.y});  //hp바 좌측고정.
	if (m_iHP == 0)
	{
		m_bDead = true;
	}
}

void HpBar::Render()
{
	if (m_fShowTime > 0.0f)
	{
		Object2D::Render();
	}
}