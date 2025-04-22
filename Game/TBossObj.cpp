#include "TBossObj.h"
void TBossObj::HitOverlap(Object* pObj, THitResult hRes)  //충돌했을떄 실행되는 콜백함수
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();

	if (OtherType == ObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile && pMissile->m_pOwnerType == Shooter::OWNER_HERO && m_state != BossState::STATE_Create && m_state != BossState::STATE_PHASE2Create
			&& m_state != BossState::STATE_Transition && m_state != BossState::STATE_PHASE2)
		{
			pMissile->m_bDead = true;
			m_HP -= pMissile->m_Data.m_iDamage;
			if (m_HP <= 0)		m_HP = 0;
			m_bAttacked = true;
			//m_eAttacked = ATTACKSTATE::STATE_OK;
		}
	}
	if (OtherType == ObjectType::Wall)
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
			SetScale({ 100,50 });
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
		rt.SetS(m_rtBossM1Frames[m_iBossM1Frame].left,
			m_rtBossM1Frames[m_iBossM1Frame].top,
			m_rtBossM1Frames[m_iBossM1Frame].right,
			m_rtBossM1Frames[m_iBossM1Frame].bottom); // 현재 프레임의 텍스처 좌표,
		switch (m_iBoss2PhaseFrame)
		{
		case 0:
			SetScale({ 85.6f,95.2f });
			break;
		case 1:
			SetScale({ 85.6f,95.2f });
			break;
		case 2:
			SetScale({ 85.6f,95.2f });
			break;
		default:
			break;
		}
		break;
	case BossState::STATE_PAUSE:
		rt.SetS(241, 442, 71, 63);
		SetScale({ 101.3f, 98.3f });
		break;
	case BossState::STATE_Attack3:
		rt.SetS(m_rtBossM2Frames[m_iBossM2Frame].left,
			m_rtBossM2Frames[m_iBossM2Frame].top,
			m_rtBossM2Frames[m_iBossM2Frame].right,
			m_rtBossM2Frames[m_iBossM2Frame].bottom); // 현재 프레임의 텍스처 좌표,
		switch (m_iBossM2Frame)
		{
		case 0:
			SetScale({ 101.3f, 98.3f });
			break;
		case 1:
			SetScale({ 78.5f,98.3f });
			break;
		default:
			break;
		}
		break;
	case BossState::STATE_Flying:
		rt.SetS(m_rtBossFlyingFrames[m_iBossFlyingFrames].left,
			m_rtBossFlyingFrames[m_iBossFlyingFrames].top,
			m_rtBossFlyingFrames[m_iBossFlyingFrames].right,
			m_rtBossFlyingFrames[m_iBossFlyingFrames].bottom); // 현재 프레임의 텍스처 좌표,
		switch (m_iBossFlyingFrames)
		{
		case 0:
			SetScale({ 82.78f, 74.9f });
			break;
		case 1:
			SetScale({ 82.78f,74.9f });
			break;
		case 2:
			SetScale({ 82.78f, 74.9f });
			break;
		case 3:
			SetScale({ 82.78f,74.9f });
			break;
		default:
			break;
		}
		break;
	case BossState::STATE_Dead:
		rt.SetS(m_rtBossDyingFrames[m_iBossDyingFrames].left,
			m_rtBossDyingFrames[m_iBossDyingFrames].top,
			m_rtBossDyingFrames[m_iBossDyingFrames].right,
			m_rtBossDyingFrames[m_iBossDyingFrames].bottom); // 현재 프레임의 텍스처 좌표,
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

	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	float fInitDistance = (m_vInitedPos - m_vPos).Length();
	TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(vMove);


	if (m_vDir.x > 0)
	{
		BossView = HeroView::RightView;
	}
	else
	{
		BossView = HeroView::LeftView;
	}

	if (m_HP > 0)
	{
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
		else if (m_state == BossState::STATE_Attack2)
		{
			if (m_bShotting)
			{
				m_iBossM1Frame = 1;
			}
			else
			{
				m_iBossM1Frame = 0;
			}
		}
		else if (m_state == BossState::STATE_Flying)
		{
			m_fCurrentTime += g_fSPF; // 시간 업데이트

			if (m_fCurrentTime >= m_fBossFlyingFrameTime)
			{
				m_fCurrentTime -= m_fBossFlyingFrameTime;
				m_iBossFlyingFrames++; // 다음 프레임으로 이동
				if (m_iBossFlyingFrames >= m_rtBossFlyingFrames.size())
				{
					m_iBossFlyingFrames = 0;
				}
			}
		}
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
			if (fHeroDistance < 600.0f || m_HP != m_iPreHP)
			{
				m_state = BossState::STATE_Attack1;
				m_vDir = { -1.0f,0.0f };
			}
		}
		else if (m_state == BossState::STATE_Attack1)
		{
			m_fSpeed = 1000.0f;

			if (m_HP < 130.0f)
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
			if (m_fPhase2CurrentTime <= 0.0f)			m_state = BossState::STATE_Attack2;
		}
		else if (m_state == BossState::STATE_Attack2)
		{
			m_fSpeed = 400;
			m_fM1Time -= g_fSPF;
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
			TVector2    vPos;
			vPos.x = m_vPos.x - m_rtScreen.vh.x;
			vPos.y = m_vPos.y;
			TVector2 vStart = vPos - vHalf;
			TVector2	vEnd = vPos + vHalf;
			TVector2    dir = { -1.0f, 0.0f };

			if (m_ftrigger < 0.0f)
			{

				m_pProjectile->AddEffect(vStart, vEnd, dir, Shooter::OWNER_BOSS1, this);
				m_bBossA2 = true;
				m_ftrigger = 0.5f;
				m_bShotting = true;
			}

			if (m_ftrigger < 0.25f && m_ftrigger>0.0f)
			{
				m_bShotting = false;
			}
			if (m_fM1Time < 0.0f)
			{
				m_state = BossState::STATE_Attack3;
				m_fM1Time = 7.0f;
				m_fM2TriggerTime = 1.5f;
			}
		}
		else if (m_state == BossState::STATE_Attack3)
		{
			m_vDir = (m_vMapCenter - m_vPos).Normal();
			if ((m_vMapCenter - m_vPos).Length() < 2.0f)
			{
				m_vDir = { 0.0f, 0.0f };
			}
			m_fM2TriggerTime -= g_fSPF;
			m_fM2Time -= g_fSPF;
			if (m_fM2TriggerTime <= 0.0f)
			{
				m_bM2Fire = true;
				m_fM2TriggerTime = 1.5f;
				m_iBossM2Frame = 0;
			}
			if (m_fM2TriggerTime < 1.0f && m_fM2TriggerTime > 0.0f)
			{
				m_iBossM2Frame = 1;
			}
			if (m_fM2Time < 0.0f)
			{
				m_state = BossState::STATE_Flying;
				m_vDir = (m_vLeftSide - m_vPos).Normal();
				m_fM2Time = 7.0f;
				m_fSpeed = 300.0f;
			}
		}
		else if (m_state == BossState::STATE_Flying)
		{

			if ((m_vLeftSide - m_vPos).Length() < 1.0f)
			{
				m_fSpeed = 1100.0f;
				m_vDir.x = 1.0f;
			}
			if (m_vPos.x > 13800.0f)
			{
				m_state = BossState::STATE_Attack2;
				m_ftrigger = 1.0f;
			}
		}

	}
	else if (m_state == BossState::STATE_Dead)
	{
		m_fSpeed = 500.0f;
		
		if ((m_vLastPos - m_vPos).Length() > 3.0f)
		{
			m_iBossDyingFrames = 0;
		}
		else
		{ 
			m_vDir = { 0.0f,0.0f };
			m_fCurrentTime += g_fSPF; // 시간 업데이트

			if (m_fCurrentTime >= m_fBossDyingFrameTime)
			{
				m_fCurrentTime -= m_fBossDyingFrameTime;
				m_iBossDyingFrames++; // 다음 프레임으로 이동
				if (m_iBossDyingFrames >= m_rtBossDyingFrames.size())
				{
					m_iBossDyingFrames = m_rtBossDyingFrames.size() - 1;
				}
			}
		}
	}


	if (m_HP <= 0)
	{
		m_state = BossState::STATE_Dead;
		m_bDeadCheck = true;
	}

	if (m_bDeadCheck &&(m_vLastPos - m_vPos).Length() > 3.0f)
	{
		m_vDir = (m_vLastPos - m_vPos).Normal();
		m_bDeadCheck = false;
	}


	m_pProjectile->Frame(m_vPos);
}

void TBossObj::Render()
{
	Object2D::Render();
	m_pProjectile->Render(m_vCamera);
}
void TBossObj::Release()
{
	Object2D::Release();
	m_pProjectile->Release();
}

void TBossObj::GetState(bool state)
{
	m_bGameState = state;
}