#include "TMonster.h"

void TMonster1::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(8.0f, 0.0f, 36.0f, 26.0f);
	switch (m_state)
	{
	case Monster1State::STATE_Move:
		break;
	case Monster1State::STATE_Attack:
		rt.SetS(m_rtMon1AttackFrames[m_iMon1AttckFrame].left,
			m_rtMon1AttackFrames[m_iMon1AttckFrame].top,
			m_rtMon1AttackFrames[m_iMon1AttckFrame].right,
			m_rtMon1AttackFrames[m_iMon1AttckFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	case Monster1State::STATE_Return:
		break;
	case Monster1State::STATE_Dead:
		rt.SetS(8, 104, 26, 30);
		break;
	default:
		break;
	}
	if (m_eCurrentView == HeroView::LeftView)
	{
		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	}
	else
	{
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	}
}
void TMonster1::Frame()
{
	if (m_vPos.x > m_pHero->m_vPos.x)
	{
		m_eCurrentView = HeroView::LeftView;
	}
	else
	{
		m_eCurrentView = HeroView::RightView;
	}




	TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(vMove);
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	float fInitDistance = (m_vInitedPos - m_vPos).Length();
	if (m_HP > 0)
	{
		if (m_state == Monster1State::STATE_Move)
		{
			m_fSpeed = 100.0f;
			if (m_vPos.x >= m_vInitedPos.x + m_iLimitedDis || m_vPos.x <= m_vInitedPos.x - m_iLimitedDis)
			{
				m_vDir.x *= -1.0f;
			}

			if (fHeroDistance < 400.0f)
			{
				m_fFindTime -= g_fSPF;
			}


			if (m_fFindTime < 0.0f || m_iPreHP != m_HP)
			{
				m_state = Monster1State::STATE_Attack;
				m_fFindTime = 1.0f;
				m_fSpeed = rand() % 50 + 200.0f;
			}
		}
		else if (m_state == Monster1State::STATE_Attack)
		{
			//m_fSpeed = 250.0f;
			m_vDir = (m_pHero->m_vPos - m_vPos).Normal();
			m_vDir.y = 0.0f;
			if (fInitDistance > 600.0f)
			{
				m_state = Monster1State::STATE_Return;
			}
			/*if (fHeroDistance > 500.0f)
			{
				m_state = Monster1State::STATE_Return;
			}*/
			m_fCurrentTime += g_fSPF; // 시간 업데이트
			if (m_fCurrentTime >= m_fMon1AttackFrameTime)
			{
				m_fCurrentTime -= m_fMon1AttackFrameTime;
				m_iMon1AttckFrame++; // 다음 프레임으로 이동
				if (m_iMon1AttckFrame >= m_rtMon1AttackFrames.size())
				{
					if (m_bLoop)
						m_iMon1AttckFrame = 0; // 반복일 경우 첫 프레임으로 이동
					else
						m_iMon1AttckFrame = m_rtMon1AttackFrames.size() - 1; // 마지막 프레임 유지
				}
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
	else
	{
		m_vDir = { 0.0f,0.0f };
		m_state = Monster1State::STATE_Dead;
		m_fDeadTime -= g_fSPF;
		m_bAbleAttack = false;
		if (m_fDeadTime < 0.0f)
		{
			m_bDead = true;
		}
		SetRotation(T_Pi * 0.4f);
	}


	SetVertexData();
}

void TMonster2::SetVertexData()
{

	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	switch (m_state)
	{
	case Monster2State::STATE_Idle:
		SetScale(58.75f, 50.0f);
		rt.SetS(10.0f, 26.0f, 46.0f, 31.0f);
		break;
	case Monster2State::STATE_Attack:
		SetScale(67.5f, 37.1f);
		rt.SetS(m_rtMon2AttackFrames[m_iMon2AttckFrame].left,
			m_rtMon2AttackFrames[m_iMon2AttckFrame].top,
			m_rtMon2AttackFrames[m_iMon2AttckFrame].right,
			m_rtMon2AttackFrames[m_iMon2AttckFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	case Monster2State::STATE_Dead:
		rt.SetS(m_rtMon2DyingFrames[m_iMon2DyingFrame].left,
			m_rtMon2DyingFrames[m_iMon2DyingFrame].top,
			m_rtMon2DyingFrames[m_iMon2DyingFrame].right,
			m_rtMon2DyingFrames[m_iMon2DyingFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	default:
		break;
	}
	if (m_eCurrentView == HeroView::RightView)
	{
		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	}
	else
	{
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	}
}
void TMonster2::Init()
{
	TNpcObj::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}
void TMonster2::Frame()
{
	if (m_vPos.x > m_pHero->m_vPos.x)
	{
		m_eCurrentView = HeroView::LeftView;
	}
	else
	{
		m_eCurrentView = HeroView::RightView;
	}
	SetVertexData();
	TNpcObj::Frame();
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	if (m_HP > 0)
	{
		if (m_state == Monster2State::STATE_Idle)
		{
			if (fHeroDistance < 500.0f)
			{
				m_state = Monster2State::STATE_Attack;
				AddPosition(0, 13.0f);
			}
		}
		else if (m_state == Monster2State::STATE_Attack)
		{
			m_ftrigger -= g_fSPF;
			if (fHeroDistance >= 640.0f)
			{
				m_state = Monster2State::STATE_Idle;
				AddPosition(0, -13.0f);
			}

			m_fCurrentTime += g_fSPF; // 시간 업데이트
			if (m_fCurrentTime >= m_fMon2AttackFrameTime)
			{
				m_fCurrentTime -= m_fMon2AttackFrameTime;
				m_iMon2AttckFrame++; // 다음 프레임으로 이동
				if (m_iMon2AttckFrame >= m_rtMon2AttackFrames.size())
				{
					m_iMon2AttckFrame = 0; 
				}
			}
			TVector2	vHalf = { 20.0f, 10.0f };
			TVector2 vStart;
			TVector2 vEnd;
			if (m_eCurrentView == HeroView::LeftView)
			{
				vStart = m_vPos - vHalf;
				vStart.x -= 20.0f;
				vEnd = m_vPos + vHalf;
				vEnd.x -= 20.f;
			}
			else
			{
				vStart = m_vPos - vHalf;
				vStart.x += 20.0f;
				vEnd = m_vPos + vHalf;
				vEnd.x += 20.0f;
			}
			
			TVector2    dir = (m_pHero->m_vPos - m_vPos).Normal();
			dir = { dir.x,0.0f };
			if (m_ftrigger < 0.0f)
			{
				m_pProjectile->AddEffect(vStart, vEnd, dir, Shooter::OWNER_MON2, this);
				m_ftrigger = 1.0f;
			}
			//Shoot();
		}
	}
	else
	{
		m_vDir = { 0.0f,0.0f };
		m_state = Monster2State::STATE_Dead;
		m_fDeadTime -= g_fSPF;
		m_bAbleAttack = false;
		if (m_fDeadTime < 0.0f)
		{
			m_bDead = true;
		}
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fMon2DyingFrameTime)
		{
			m_fCurrentTime -= m_fMon2DyingFrameTime;
			m_iMon2DyingFrame++; // 다음 프레임으로 이동
			if (m_iMon2DyingFrame >= m_rtMon2DyingFrames.size())
			{
				m_iMon2DyingFrame = m_rtMon2DyingFrames.size()-1; // 반복일 경우 첫 프레임으로 이동
			}
		}
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
	switch (m_state)
	{
	case Monster3State::STATE_Idle:
		rt.SetS(90.0f, 6.0f, 38.0f, 30.0f);
		break;
	case Monster3State::STATE_Attack:
		rt.SetS(m_rtMon3AttackFrames[m_iMon3AttckFrame].left,
			m_rtMon3AttackFrames[m_iMon3AttckFrame].top,
			m_rtMon3AttackFrames[m_iMon3AttckFrame].right,
			m_rtMon3AttackFrames[m_iMon3AttckFrame].bottom); 
		break;
	case Monster3State::STATE_Dead:
		rt.SetS(132.0f, 6.0f, 38.0f, 30.0f);
		break;
	default:
		break;
	}
	
	if (m_eCurrentView == HeroView::LeftView)
	{
		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	}
	else
	{
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	}
}
void TMonster3::Init()
{
	TNpcObj::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}
void TMonster3::Frame()
{
	if (m_vPos.x > m_pHero->m_vPos.x)
	{
		m_eCurrentView = HeroView::LeftView;
	}
	else
	{
		m_eCurrentView = HeroView::RightView;
	}
	SetVertexData();
	TNpcObj::Frame();
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	if (m_HP > 0)
	{
		if (m_state == Monster3State::STATE_Idle)
		{
			if (fHeroDistance < 500.0f)
			{
				m_state = Monster3State::STATE_Attack;
			}
		}
		else if (m_state == Monster3State::STATE_Attack)
		{
			m_ftrigger -= g_fSPF;
			if (fHeroDistance >= 500.0f)
			{
				m_state = Monster3State::STATE_Idle;
			}

			TVector2	vHalf = { 25.0f, 25.0f };
			TVector2	vStart = m_vPos - vHalf;
			TVector2	vEnd = m_vPos + vHalf;
			TVector2    dir;
			if (m_eCurrentView == HeroView::LeftView)
			{
				dir = { -1.0f, -1.0f };
			}
			else
			{
				dir = { 1.0f, -1.0f };
			}

			if (m_ftrigger < 0.0f)
			{
				m_pProjectile->AddEffect(vStart, vEnd, dir, Shooter::OWNER_MON3, this);
				m_ftrigger = 1.0f;
			}

			m_fCurrentTime += g_fSPF; // 시간 업데이트
			if (m_fCurrentTime >= m_fMon3AttackFrameTime)
			{
				m_fCurrentTime -= m_fMon3AttackFrameTime;
				m_iMon3AttckFrame++; // 다음 프레임으로 이동
				if (m_iMon3AttckFrame >= m_rtMon3AttackFrames.size())
				{
					m_iMon3AttckFrame = 0;
				}
			}
			//Shoot();
		}
	}
	else
	{
		m_vDir = { 0.0f,0.0f };
		m_state = Monster3State::STATE_Dead;
		m_fDeadTime -= g_fSPF;
		m_bAbleAttack = false;
		if (m_fDeadTime < 0.0f)
		{
			m_bDead = true;
		}
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
