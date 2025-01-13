#include "TMonster.h"

void TMonster1::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(8.0f, 0.0f, 35.0f, 26.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TMonster1::Frame()
{
	TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(vMove);
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	float fInitDistance = (m_vInitedPos - m_vPos).Length();
	if (m_state == Monster1State::STATE_Move)
	{
		//if (m_vPos.x <= m_vInitedPos.x + m_iLimitedDis && m_vPos.x >= m_vInitedPos.x - m_iLimitedDis)
		m_fSpeed = 100.0f;
		if (m_vPos.x >= m_vInitedPos.x + m_iLimitedDis || m_vPos.x <= m_vInitedPos.x - m_iLimitedDis)
		{
			m_vDir.x *= -1.0f;
		}
	
		if (fHeroDistance < 400.0f)
		{
			m_state = Monster1State::STATE_Attack;
		}
	}
	else if (m_state == Monster1State::STATE_Attack)
	{
		m_fSpeed = 250.0f;
		m_vDir = (m_pHero->m_vPos - m_vPos).Normal();
		m_vDir.y = 0.0f;
		if (fInitDistance > 600.0f)
		{
			m_state = Monster1State::STATE_Return;
		}
		if (fHeroDistance > 500.0f)
		{
			m_state = Monster1State::STATE_Return;
		}
	}
	else if (m_state == Monster1State::STATE_Return)
	{
		m_fSpeed = 500.0f;
		m_vDir = (m_vInitedPos - m_vPos).Normal();
		if (m_HP < 50)
		{
			m_HP++;
		}
		if (fInitDistance < 1.0f)
		{
			m_state = Monster1State::STATE_Move;
		}
	}
}

void TMonster2::SetVertexData()
{

	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(10.0f, 26.0f, 57.0f, 57.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	
}
void TMonster2::Init()
{
	TNpcObj::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}
void TMonster2::Frame()
{
	TNpcObj::Frame();
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	if (m_state == Monster2State::STATE_Idle)
	{
		if (fHeroDistance < 640.0f)
		{
			m_state = Monster2State::STATE_Attack;
		}
	}
	else if (m_state == Monster2State::STATE_Attack)
	{
		m_ftrigger -= g_fSPF;
		if (fHeroDistance >= 640.0f)
		{
			m_state = Monster2State::STATE_Idle;
		}
		
		TVector2	vHalf = { 30.0f, 30.0f };
		TVector2	vStart = m_vPos - vHalf;
		TVector2	vEnd = m_vPos + vHalf;
		TVector2    dir = (m_pHero->m_vPos - m_vPos).Normal();
				
		if (m_ftrigger < 0.0f)
		{
			m_pProjectile->AddEffect(vStart, vEnd, dir, Shooter::OWNER_MON2, this);
			m_ftrigger = 1.0f;
		}
		//Shoot();
	}
	
	m_pProjectile->Frame(m_vPos);
}
void TMonster2::Render()
{
	TObject2D::Render();
	m_pProjectile->Render(m_vCamera);
}
void TMonster2::Release()
{
	TObject2D::Release();
	m_pProjectile->Release();
}


void TMonster3::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(3.0f, 7.0f, 27.0f, 36.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TMonster3::Init()
{
	TNpcObj::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}
void TMonster3::Frame()
{
	TNpcObj::Frame();
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	if (m_state == Monster2State::STATE_Idle)
	{
		if (fHeroDistance < 640.0f)
		{
			m_state = Monster2State::STATE_Attack;
		}
	}
	else if (m_state == Monster2State::STATE_Attack)
	{
		m_ftrigger -= g_fSPF;
		if (fHeroDistance >= 640.0f)
		{
			m_state = Monster2State::STATE_Idle;
		}

		TVector2	vHalf = { 25.0f, 25.0f };
		TVector2	vStart = m_vPos - vHalf;
		TVector2	vEnd = m_vPos + vHalf;
		TVector2    dir = { 0.0f, -1.0f };

		if (m_ftrigger < 0.0f)
		{
			m_pProjectile->AddEffect(vStart, vEnd, dir, Shooter::OWNER_MON3,this);
			m_ftrigger = 1.0f;
		}
		//Shoot();
	}

	m_pProjectile->Frame(m_vPos);
}
void TMonster3::Render()
{
	TObject2D::Render();
	m_pProjectile->Render(m_vCamera);
}
void TMonster3::Release()
{
	TObject2D::Release();
	m_pProjectile->Release();
}
