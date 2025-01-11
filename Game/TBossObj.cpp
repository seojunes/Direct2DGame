#include "TBossObj.h"
void TBossObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(0.0f, 0.0f, 63.0f, 57.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TBossObj::Frame()
{
	if (!m_bGameState)		return;
	TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(vMove);
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	float fInitDistance = (m_vInitedPos - m_vPos).Length();
	if (m_state == BossState::STATE_Create)
	{

	}
	else if (m_state == BossState::STATE_Move)
	{
		//if (m_vPos.x <= m_vInitedPos.x + m_iLimitedDis && m_vPos.x >= m_vInitedPos.x - m_iLimitedDis)
		m_fSpeed = 100.0f;
		if (m_vPos.x >= m_vInitedPos.x + m_iLimitedDis || m_vPos.x <= m_vInitedPos.x - m_iLimitedDis)
		{
			m_vDir.x *= -1.0f;
		}

		if (fHeroDistance < 200.0f)
		{
			m_state = BossState::STATE_Attack;
		}
	}
	else if (m_state == BossState::STATE_Attack)
	{
		m_fSpeed = 250.0f;
		m_vDir = (m_pHero->m_vPos - m_vPos).Normal();
		m_vDir.y = 0.0f;
		if (fInitDistance > 600.0f)
		{
			m_state = BossState::STATE_Return;
		}
		if (fHeroDistance > 300.0f)
		{
			m_state = BossState::STATE_Return;
		}
	}
	else if (m_state == BossState::STATE_Return)
	{
		m_fSpeed = 500.0f;
		m_vDir = (m_vInitedPos - m_vPos).Normal();
		if (fInitDistance < 1.0f)
		{
			m_state = BossState::STATE_Move;
		}
	}
}

void TBossObj::GetState(bool state)
{
	m_bGameState = state;
}