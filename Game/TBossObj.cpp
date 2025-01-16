#include "TBossObj.h"
void TBossObj::HitOverlap(TObject* pObj, THitResult hRes)  //충돌했을떄 실행되는 콜백함수
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();

	if (OtherType == TObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile && pMissile->m_pOwnerType == Shooter::OWNER_HERO && m_state != BossState::STATE_Create)
		{
			pMissile->m_bDead = true;
			m_HP -= pMissile->m_Data.m_iDamage;
			if (m_HP <= 0)		m_HP = 0;
			m_bAttacked = true;
			//m_eAttacked = ATTACKSTATE::STATE_OK;
		}
	}
	if (OtherType == TObjectType::Wall)
	{
		auto pWall = dynamic_cast<TCollisionManager*>(pObj);
		if (pWall)
		{
			if (m_vPos.y > pWall->m_rtScreen.v1.y)
			{
				if (m_vPos.x < pWall->m_rtScreen.v1.x)
				{
					m_vDir.x *= -1.0f;
				}
				else
				{
					m_vDir.x *= -1.0f;
				}
			}
			if (m_rtScreen.v2.y > pWall->m_rtScreen.v1.y && m_rtScreen.v1.y < pWall->m_rtScreen.v1.y)
			{
				m_vPos.y = pWall->m_rtScreen.v1.y - m_rtScreen.vh.y;
			}
		}
	}
}
void TBossObj::SetVertexData()
{
	TRect rt;
	switch (m_state)
	{
	case BossState::STATE_Idle:
		//m_rtBossIdleFrames
		rt.SetS(m_rtBossIdleFrames[m_iBossIdleFrame].left,
			m_rtBossIdleFrames[m_iBossIdleFrame].top,
			m_rtBossIdleFrames[m_iBossIdleFrame].right,
			m_rtBossIdleFrames[m_iBossIdleFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	case BossState::STATE_Create:
		rt.SetS(m_rtBossCreateFrames[m_iBossCreateFrame].left,
			m_rtBossCreateFrames[m_iBossCreateFrame].top,
			m_rtBossCreateFrames[m_iBossCreateFrame].right,
			m_rtBossCreateFrames[m_iBossCreateFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	case BossState::STATE_Move:
		break;
	case BossState::STATE_Attack1:
		rt.SetS(m_rtBossAttack1Frames[m_iBossAttack1Frame].left,
			m_rtBossAttack1Frames[m_iBossAttack1Frame].top,
			m_rtBossAttack1Frames[m_iBossAttack1Frame].right,
			m_rtBossAttack1Frames[m_iBossAttack1Frame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	case BossState::STATE_Transition:
		rt.SetS(m_rtBossTransFrames[m_iBossTransFrame].left,
			m_rtBossTransFrames[m_iBossTransFrame].top,
			m_rtBossTransFrames[m_iBossTransFrame].right,
			m_rtBossTransFrames[m_iBossTransFrame].bottom); // 현재 프레임의 텍스처 좌표,
		switch (m_iBossTransFrame)
		{
		case 0:
			SetScale({ 100,50});
			break;
		case 1:
			SetScale({ 74, 56.5f });
			break;
		case 2:
			SetScale({ 48.5f,35 });
			break;
		default:
			break;
		}
		break;
	case BossState::STATE_PHASE2Create:
		rt.SetS(354, 123, 34, 75);
		SetScale({ 48.5f,119 });
		break;
	case BossState::STATE_PHASE2:
		rt.SetS(m_rtBoss2PhaseFrames[m_iBoss2PhaseFrame].left,
			m_rtBoss2PhaseFrames[m_iBoss2PhaseFrame].top,
			m_rtBoss2PhaseFrames[m_iBoss2PhaseFrame].right,
			m_rtBoss2PhaseFrames[m_iBoss2PhaseFrame].bottom); // 현재 프레임의 텍스처 좌표,
		switch (m_iBoss2PhaseFrame)
		{
		case 0:
			SetScale({ 48.5f , 95.2f });
			break;
		case 1:
			SetScale({ 54.2f, 95.2f });
			break;
		case 2:
			SetScale({ 75.6f, 95.2f });
			break;
		default:
			break;
		}
		break;
		break;
	case BossState::STATE_Attack2:
		rt.SetS(400, 283, 60,60);
		SetScale({ 85.6f,95.2f });
		break;
	case BossState::STATE_Return:
		break;
	default:
		break;
	}
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	if (BossView == HeroView::LeftView)
	{
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	}
	else
	{

		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	}
}


void TBossObj::Init()
{
	TNpcObj::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}

void TBossObj::Frame()
{
	if (!m_bGameState)		return;
	
	SetVertexData();

	if (m_vDir.x > 0)
	{
		BossView = HeroView::RightView;
	}
	else
	{
		BossView = HeroView::LeftView;
	}

	if (m_state == BossState::STATE_Idle)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fBossIdleFrameTime)
		{
			m_fCurrentTime -= m_fBossIdleFrameTime;
			m_iBossIdleFrame++; // 다음 프레임으로 이동
			if (m_iBossIdleFrame >= m_rtBossIdleFrames.size())
			{
				if (m_bLoop)
					m_iBossIdleFrame = 0; // 반복일 경우 첫 프레임으로 이동
				else
					m_iBossIdleFrame = m_rtBossIdleFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	else if (m_state == BossState::STATE_Create)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트

		if (m_fCurrentTime >= m_fBossCreateFrameTime)
		{
			m_fCurrentTime -= m_fBossCreateFrameTime;
			m_iBossCreateFrame++; // 다음 프레임으로 이동
			if (m_iBossCreateFrame >= m_rtBossCreateFrames.size())
			{
				m_iBossCreateFrame = m_rtBossCreateFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	else if (m_state == BossState::STATE_Attack1)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트

		if (m_fCurrentTime >= m_fBossAttack1FrameTime)
		{
			m_fCurrentTime -= m_fBossAttack1FrameTime;
			m_iBossAttack1Frame++; // 다음 프레임으로 이동
			if (m_iBossAttack1Frame >= m_rtBossAttack1Frames.size())
			{
				m_iBossAttack1Frame = 0; // 반복일 경우 첫 프레임으로 이동
			}
		}
	}
	else if (m_state == BossState::STATE_Transition)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트

		if (m_fCurrentTime >= m_fBossTransFrameTime)
		{
			m_fCurrentTime -= m_fBossTransFrameTime;
			m_iBossTransFrame++; // 다음 프레임으로 이동
			if (m_iBossTransFrame >= m_rtBossTransFrames.size())
			{
				m_iBossTransFrame = m_rtBossTransFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	else if (m_state == BossState::STATE_PHASE2)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트

		if (m_fCurrentTime >= m_fBoss2PhaseFrameTime)
		{
			m_fCurrentTime -= m_fBoss2PhaseFrameTime;
			m_iBoss2PhaseFrame++; // 다음 프레임으로 이동
			if (m_iBoss2PhaseFrame >= m_rtBoss2PhaseFrames.size())
			{
				m_iBoss2PhaseFrame = m_rtBoss2PhaseFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}


	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	float fInitDistance = (m_vInitedPos - m_vPos).Length();
	TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(vMove);
	if (m_state == BossState::STATE_Create)
	{
		m_fSpeed = 0.0f;
		m_fCreateTime -= g_fSPF;
		if (m_fCreateTime <= 0.0f)
		{
			m_state = BossState::STATE_Idle;
		}
	}
	else if (m_state == BossState::STATE_Idle)
	{
		m_vDir = { 0.0f,0.0f };
		if (fHeroDistance < 600.0f )
		{
			m_state = BossState::STATE_Attack1;
			m_vDir = { -1.0f,0.0f };
		}
	}
	else if (m_state == BossState::STATE_Attack1)
	{
		m_fSpeed = 500.0f;
		
		if (m_HP < 180.0f)
		{
			m_fSpeed = 300.0f;
			m_vDir = (m_vInitedPos - m_vPos).Normal();
			if (fInitDistance < 1.0f)
			{
				m_vDir = { 0.0f,0.0f };
				m_state = BossState::STATE_Transition;
			}
		}
		
		//if (m_rtScreen.v2.y >= 843.5f)
		//{
		//	m_vDir = { 0.0f, 0.0f };
		//}
		//m_ftrigger -= g_fSPF;
		//m_fNextState -= g_fSPF;
		//TVector2	vHalf = { 25.0f, 25.0f };
		//TVector2	vStart = m_vPos - vHalf;
		//TVector2	vEnd = m_vPos + vHalf;
		//TVector2    dir = { 0.0f, -1.0f };
		//m_MissleDirList = {

		//	{1.0f, 0.0f},
		//	{1.0f, 1.0f},
		//	{0.0f, 1.0f},
		//	{-1.0f, 1.0f},
		//	{-1.0f, 0.0f},
		//	{-1.0f, -1.0f},
		//	{0.0f, -1.0f},
		//	{1.0f, -1.0f},
		//};
		////INT index = rand() % 8;
		//if (m_ftrigger < 0.0f)
		//{
		//	for (int num = 0; num < 8; num++)
		//	{
		//		m_pProjectile->AddEffect(vStart, vEnd, m_MissleDirList[num], Shooter::OWNER_BOSS1, this);
		//		m_pProjectile->SetRotation(T_Pi / 4.0f);
		//	}
		//	m_ftrigger = 1.0f;
		//}
		//if (m_fNextState < 0.0f)
		//{
		//	m_state = BossState::STATE_Attack2;
		//	m_fNextState = 5.0f;
		//}

	}
	else if (m_state == BossState::STATE_Transition)
	{
		m_fTransTime -= g_fSPF;
		if (m_fTransTime > 0.0f)
		{
			m_vDir = { 0.0f, -1.0f };
			m_fSpeed = 300.0f;
		}
		else
		{
			m_vDir = { 0.0f , +1.0f };
			m_state = BossState::STATE_PHASE2Create;
		}
	}
	else if (m_state == BossState::STATE_PHASE2Create)
	{
		if (m_vPos.y >= 720.0f)
		{
			m_state = BossState::STATE_PHASE2;
			m_bHealing = true;
		}
	}
	else if (m_state == BossState::STATE_PHASE2)
	{
		m_fPhase2CurrentTime -= g_fSPF;
		if (m_bHealing == true)
		{
			m_HP = 200;
			m_bHealing = false;
		}
		if(m_fPhase2CurrentTime<= 0.0f)			m_state = BossState::STATE_Attack2;
	}
	else if (m_state == BossState::STATE_Attack2)
	{
		if (m_vPos.x < 13900.0f)
		{
			m_vDir = { 1.0f,0.0f };
		}
		else
		{
			if (m_vPos.y <= 400.0f)
			{
				m_vDir = { 0.0f, +1.0f };
			}
			else if (m_vPos.y > 720.0f)
			{
				m_vDir = { 0.0f, -1.0f };
			}
		}
		m_ftrigger -= g_fSPF;
		TVector2	vHalf = { 25.0f, 25.0f };
		TVector2	vStart = m_vPos - vHalf;
		TVector2	vEnd = m_vPos + vHalf;
		TVector2    dir = { -1.0f, 0.0f };
		
		if (m_ftrigger < 0.0f)
		{
			
			m_pProjectile->AddEffect(vStart, vEnd, dir, Shooter::OWNER_BOSS1, this);
			
			
			m_ftrigger = 1.0f;
		}
		/*if (m_fNextState < 0.0f)
		{
			m_state = BossState::STATE_Attack2;
			m_fNextState = 5.0f;
		}*/

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