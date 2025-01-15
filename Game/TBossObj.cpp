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


void TBossObj::Init()
{
	TNpcObj::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
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
		m_fSpeed = 350.0f;
		m_vDir = (m_vMapCenter - m_vPos).Normal();

		if ((m_vMapCenter - m_vPos).Length() < 1.0f)
		{
			m_state = BossState::STATE_Attack1;
		}
	}
	else if (m_state == BossState::STATE_Attack1)
	{
		m_fSpeed = 100.0f;
		m_vDir = {0.0f, 1.0f };
		if (m_rtScreen.v2.y >= 843.5f)
		{
			m_vDir = { 0.0f, 0.0f };
		}
		m_ftrigger -= g_fSPF;
		m_fNextState -= g_fSPF;
		TVector2	vHalf = { 25.0f, 25.0f };
		TVector2	vStart = m_vPos - vHalf;
		TVector2	vEnd = m_vPos + vHalf;
		TVector2    dir = { 0.0f, -1.0f };
		m_MissleDirList = {
			
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f},
			{-1.0f, 1.0f},
			{-1.0f, 0.0f},
			{-1.0f, -1.0f},
			{0.0f, -1.0f},
			{1.0f, -1.0f},
		};
		//INT index = rand() % 8;
		if (m_ftrigger < 0.0f)
		{
			for (int num = 0; num < 8; num++)
			{
				m_pProjectile->AddEffect(vStart, vEnd, m_MissleDirList[num], Shooter::OWNER_BOSS1, this);
				m_pProjectile->SetRotation(T_Pi / 4.0f);
			}
			m_ftrigger = 1.0f;
		}
		if (m_fNextState < 0.0f)
		{
			m_state = BossState::STATE_Attack2;
			m_fNextState = 5.0f;
		}

	}
	else if (m_state == BossState::STATE_Attack2)
	{
		m_fSpeed = 500.0f;
		m_vDir = (m_vInitedPos - m_vPos).Normal();
		if (fInitDistance < 1.0f)
		{
			m_state = BossState::STATE_Move;
		}
	}

	m_pProjectile->Frame(m_vPos);
}

void TBossObj::Render()
{
	TObject2D::Render();
	m_pProjectile->Render(m_vCamera);
}
void TBossObj::Release()
{
	TObject2D::Release();
	m_pProjectile->Release();
}

void TBossObj::GetState(bool state)
{
	m_bGameState = state;
}