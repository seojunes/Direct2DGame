#include "THeroObj.h"
#include "Device.h"
#include "TWorld.h"

void    THeroObj::HitOverlap(Object* pObj, HitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();
	//Hero가 쏜 미사일이 아닌 미사일에 닿으면 미사일데미지만큼 피 깍임
	if (OtherType == ObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile && pMissile->m_pOwnerType != Shooter::OWNER_HERO && !m_bInvincible && pMissile->m_Data.m_pOwner->m_bDead ==false)
		{
			TakeDamage(pMissile->m_Data.m_iDamage);
			pMissile->m_bDead = true;
		}
	}
	//포탈에 닿으면 보스방으로 이동가능
	if (OtherType == ObjectType::Portal)
	{
		m_BossMoving = BossRoomMovingState::STATE_ABLE;
	}
	else
	{
		m_BossMoving = BossRoomMovingState::STATE_UNABLE;
	}

	// 몬스터와 부딪쳤을때, 데미지
	if (OtherType == ObjectType::Npc)
	{
		auto pNpc = dynamic_cast<TNpcObj*>(pObj);
		if(pNpc->m_bAbleAttack)		TakeDamage(10);
	}
	else if (OtherType == ObjectType::Boss)
	{
		TakeDamage(20);
	}
}

void THeroObj::Init()
{
	Object2D::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}

void THeroObj::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtWalkFrames.resize(SpriteList[0].size());
	m_rtJumpFrames.resize(SpriteList[1].size());
	m_rtIdleFrames.resize(SpriteList[2].size());
	m_rtShotFrames.resize(SpriteList[3].size());
	m_rtShotNRunFrames.resize(SpriteList[4].size());
	m_rtVictoryFrames.resize(SpriteList[5].size());
	m_rtRadderFrames.resize(SpriteList[6].size());
	m_rtShotNJumpFrames.resize(SpriteList[7].size());

	m_rtWalkFrames = SpriteList[0];
	m_rtJumpFrames = SpriteList[1];
	m_rtIdleFrames = SpriteList[2];
	m_rtShotFrames = SpriteList[3];
	m_rtShotNRunFrames = SpriteList[4];
	m_rtVictoryFrames = SpriteList[5];
	m_rtRadderFrames = SpriteList[6];
	m_rtShotNJumpFrames = SpriteList[7];
}

void THeroObj::TakeDamage(int damage)
{
	if (m_bInvincible == false && m_bEnd == false) // 무적 상태가 아닐 때만 데미지를 받음
	{
		m_HeroHP -= damage;

		// 무적 상태 시작
		m_bInvincible = true;
		m_fInvincibleTime = m_fMaxInvincibleTime;
		m_bAttacked = true;
	}

}

void THeroObj::GetGroundH(float height)
{
	m_fGroundY = height;
}
void THeroObj::Frame()
{

	//if (m_HeroHP <= 0)
	//{
	//	m_HeroHP = 0;
	//	m_bDead = true; // 히어로 사망 처리
	//}

	if (m_bInvincible)
	{
		m_fInvincibleTime -= g_fSPF;
		if (m_fInvincibleTime <= 0.0f)
		{
			m_bInvincible = false; // 무적 상태 해제
			m_fInvincibleTime = 0.0f;
		}
	}
	TVector2 vAdd;
	if (m_fShootingMotionTime >= 0.0f)
	{
		m_fShootingMotionTime -= g_fSPF;
	}
	else
	{
		
		m_bIsShooting = false;
	}
	if (m_CurrentState != HeroState::Victory)
	{
		if (m_rtScreen.v2.y >= m_fGroundY)
		{
			m_CurrentState = HeroState::Idle;
		}   
	}
	
	// 점프효과 구현
	if (m_bIsJumping)
	{
		m_fVerticalSpeed -= m_fGravity * g_fSPF;
		m_vPos.y -= m_fVerticalSpeed * g_fSPF;
		if (m_vPos.y >= m_fGroundY)
		{
			//m_vPos.y = m_fGroundY;
			m_bIsJumping = false;
			m_fVerticalSpeed = 0.0f;
			m_iJumpingCount = 0;
			//m_CurrentState = HeroState::Idle;
		}
	}
	// 키 입력단 
	if (m_bKeyinput)
	{
		if (g_GameKey.dwAkey == KEY_HOLD)
		{
			m_fGroundY = 1800.0f;
			vAdd.x -= m_fSpeed * g_fSPF;
			if (m_CurrentState != HeroState::Jump)
			{
				m_CurrentState = HeroState::LeftRun;
			}
			m_CurrentView = HeroView::LeftView;

		}
		if (g_GameKey.dwDkey == KEY_HOLD)
		{
			m_fGroundY = 1800.0f;
			vAdd.x += m_fSpeed * g_fSPF;
			if (m_CurrentState != HeroState::Jump)
			{
				m_CurrentState = HeroState::RightRun;
			}
			m_CurrentView = HeroView::RightView;

		}
		if (g_GameKey.dwWkey == KEY_PUSH && m_iJumpingCount < m_MaxJunp)//&& !m_bIsJumping)
		{
			m_fGroundY = 1800.0f;
			m_bIsJumping = true;
			m_fVerticalSpeed = m_fJumpSpeed;
			m_iJumpingCount++;
			m_CurrentState = HeroState::Jump; // Jump 상태 설정.
		}

		
		
		if (m_CurrentState == HeroState::Radder)
		{
			if (g_GameKey.dwSkey == KEY_HOLD)
			{
				vAdd.y += m_fSpeed * g_fSPF;
			}
			if (g_GameKey.dwWkey == KEY_HOLD)
			{
				vAdd.y -= m_fSpeed * g_fSPF;
			}
		}
		else
		{
			if (g_GameKey.dwSkey == KEY_PUSH)
			{
				m_CurrentState == HeroState::Jump;
				m_fGravity = 980.0f;
			}
		}
		
		if (g_GameKey.dwSpace == KEY_PUSH)
		{
			m_fChargingTime = 0.0f;
		}

		if (g_GameKey.dwSpace == KEY_HOLD)
		{
			// 키를 누르고 있는 동안 충전 시간 증가
			m_fChargingTime += g_fSPF;
			m_bCharging = true;
		}

		if (g_GameKey.dwSpace == KEY_UP)
		{
			m_bCharging = false;
			// 키를 떼는 순간 발사
			TVector2 vHalf;
			TVector2 vStart;
			TVector2 vEnd;

			if (m_fChargingTime < 0.5f)
			{
				// 일반 미사일
				vHalf = { 20.0f, 20.0f };
				vStart = m_vPos - vHalf;
				vEnd = m_vPos + vHalf;
				if (m_CurrentView == HeroView::RightView)
				{
					vStart.x += m_rtScreen.vh.x;
					vEnd.x += m_rtScreen.vh.x;
				}
				else
				{
					vStart.x -= m_rtScreen.vh.x;
					vEnd.x -= m_rtScreen.vh.x;
				}
				
				
				m_bOnCharing = false;
			}
			else
			{
				// 차징샷
				vHalf = { 25.0f, 40.0f };
				vStart = m_vPos - vHalf;
				vEnd = m_vPos + vHalf;
				m_bOnCharing = true;
			}

			if (m_CurrentView == RightView)								//바라보는 방향에 따라서 발사체 생성.
			{
				m_pProjectile->AddEffect(vStart, vEnd, m_vRightDir, Shooter::OWNER_HERO, this, m_bOnCharing);
			}
			else
			{
				m_pProjectile->AddEffect(vStart, vEnd, m_vLeftDir, Shooter::OWNER_HERO, this, m_bOnCharing);
			}
			// 충전 시간 초기화
			//m_pProjectile->
			m_fChargingTime = 0.0f;
			m_bIsShooting = true;
			m_fShootingMotionTime = m_fMaxMotionTime;
		}
	}
	
	
	// 애니메이션 업데이트
	if (m_CurrentState == HeroState::Idle)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fIdleFrameTime)
		{
			m_fCurrentTime -= m_fIdleFrameTime;
			m_iIdleFrame++; // 다음 프레임으로 이동
			if (m_iIdleFrame >= m_rtIdleFrames.size())
			{
				if (m_bLoop)
					m_iIdleFrame = 0; // 반복일 경우 첫 프레임으로 이동
				else
					m_iIdleFrame = m_rtIdleFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	if ((g_GameKey.dwAkey == KEY_HOLD || g_GameKey.dwDkey == KEY_HOLD) && m_CurrentState != HeroState::Jump)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fWalkFrameTime)
		{
			m_fCurrentTime -= m_fWalkFrameTime;
			m_iWalkFrame++; // 다음 프레임으로 이동
			if (m_iWalkFrame >= m_rtWalkFrames.size())
			{
				if (m_bLoop)
					m_iWalkFrame = 0; // 반복일 경우 첫 프레임으로 이동
				else
					m_iWalkFrame = m_rtWalkFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	//if (m_CurrentState == HeroState::Jump)
	//{
	//	m_fCurrentTime += g_fSPF; // 시간 업데이트
	//	if (m_fCurrentTime >= m_fJumpFrameTime)
	//	{
	//		m_fCurrentTime -= m_fJumpFrameTime;
	//		m_iJumpFrame++;
	//		if (m_fVerticalSpeed > 0)
	//		{
	//			if (m_iJumpFrame >= 2)
	//			{
	//				m_iJumpFrame = 1;
	//			}
	//		}
	//		else
	//		{
	//			if (m_iJumpFrame >= m_rtJumpFrames.size())
	//			{
	//				m_iJumpFrame = m_rtJumpFrames.size() - 1; // 마지막 프레임 유지
	//			}
	//		}
	//	}
	//}
	if (m_CurrentState == HeroState::Victory)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fVictoryFrameTime)
		{
			m_fCurrentTime -= m_fVictoryFrameTime;
			m_iVictoryFrame++; // 다음 프레임으로 이동
			if (m_iVictoryFrame >= m_rtVictoryFrames.size())
			{
				if (m_bLoop)
					m_iVictoryFrame = 0; // 반복일 경우 첫 프레임으로 이동
				else
					m_iVictoryFrame = m_rtVictoryFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	if (m_CurrentState == HeroState::Radder)
	{
		if (g_GameKey.dwSkey == KEY_HOLD)
		{
			m_fCurrentTime += g_fSPF; // 시간 업데이트
			if (m_fCurrentTime >= m_fRadderFrameTime)
			{
				m_fCurrentTime -= m_fRadderFrameTime;
				m_iRadderFrame++; // 다음 프레임으로 이동
				if (m_iRadderFrame >= m_rtRadderFrames.size())
				{
					if (m_bLoop)
						m_iRadderFrame = 0; // 반복일 경우 첫 프레임으로 이동
					else
						m_iRadderFrame = m_rtRadderFrames.size() - 1; // 마지막 프레임 유지
				}
			}
		}
		else if (g_GameKey.dwWkey == KEY_HOLD)
		{
			m_fCurrentTime += g_fSPF; // 시간 업데이트
			if (m_fCurrentTime >= m_fRadderFrameTime)
			{
				m_fCurrentTime -= m_fRadderFrameTime;
				m_iRadderFrame--; // 이전 프레임으로 이동
				if (m_iRadderFrame >= m_rtRadderFrames.size())
				{
					if (m_bLoop)
						m_iRadderFrame = 0; // 반복일 경우 첫 프레임으로 이동
					else
						m_iRadderFrame = m_rtRadderFrames.size() - 1; // 마지막 프레임 유지
				}
			}
		}
	}

	if (m_bCharging == false)
	{
		m_fAlpha = 1.0f;
	}

	
	if (m_bInvincible == true)  // 깜빡임 효과
	{
		if (m_fAlpha <= 1.0f)
			m_fAlpha = cosf(g_fGT * 5.0f) * 0.25f + 0.75f;
	}

	AddPosition(vAdd);
	SetVertexData();


	m_pProjectile->Frame(m_vPos);
}
void THeroObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	Object2D::SetVertexData();

	Rect rt;


	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;

	switch (m_CurrentState)
	{
	case HeroState::RightRun:
	case HeroState::LeftRun:
		if (m_bIsShooting == false)
		{
			rt.SetS(m_rtWalkFrames[m_iWalkFrame].left,
				m_rtWalkFrames[m_iWalkFrame].top,
				m_rtWalkFrames[m_iWalkFrame].right,
				m_rtWalkFrames[m_iWalkFrame].bottom); // 현재 프레임의 텍스처 좌표,
			break;
		}
		else
		{
			rt.SetS(m_rtShotNRunFrames[m_iWalkFrame].left,
				m_rtShotNRunFrames[m_iWalkFrame].top,
				m_rtShotNRunFrames[m_iWalkFrame].right,
				m_rtShotNRunFrames[m_iWalkFrame].bottom); // 현재 프레임의 텍스처 좌표,
			break;
		}
	case HeroState::Jump:
		if (m_fVerticalSpeed > 0)
		{
			m_iJumpFrame = 0;
		}
		else
		{
			m_iJumpFrame = 1;
		}
		if (m_bIsShooting == false)
		{
			rt.SetS(m_rtJumpFrames[m_iJumpFrame].left,
				m_rtJumpFrames[m_iJumpFrame].top,
				m_rtJumpFrames[m_iJumpFrame].right,
				m_rtJumpFrames[m_iJumpFrame].bottom);
			break;
		}
		else
		{
			rt.SetS(m_rtShotNJumpFrames[0].left,
				m_rtShotNJumpFrames[0].top,
				m_rtShotNJumpFrames[0].right,
				m_rtShotNJumpFrames[0].bottom);
			break;
		}
	case HeroState::Idle:
		if(!m_bIsShooting)
		{
			rt.SetS(m_rtIdleFrames[m_iIdleFrame].left,
				m_rtIdleFrames[m_iIdleFrame].top,
				m_rtIdleFrames[m_iIdleFrame].right,
				m_rtIdleFrames[m_iIdleFrame].bottom);
			break;
		}
		else
		{
			rt.SetS(m_rtShotFrames[0].left,
				m_rtShotFrames[0].top,
				m_rtShotFrames[0].right,
				m_rtShotFrames[0].bottom);
			break;
		}
	case HeroState::Radder:
		if (m_rtScreen.v2.y <= m_pRadder->m_rtScreen.v1.y+ 10.0f)
		{
			rt.SetS(m_rtIdleFrames[0].left,
				m_rtIdleFrames[0].top,
				m_rtIdleFrames[0].right,
				m_rtIdleFrames[0].bottom);
		}
		else
		{
			rt.SetS(m_rtRadderFrames[m_iRadderFrame].left,
				m_rtRadderFrames[m_iRadderFrame].top,
				m_rtRadderFrames[m_iRadderFrame].right,
				m_rtRadderFrames[m_iRadderFrame].bottom);
		}
		break;
	case HeroState::Victory:
	{
		m_CurrentView = HeroView::RightView;
		rt.SetS(m_rtVictoryFrames[m_iVictoryFrame].left,
			m_rtVictoryFrames[m_iVictoryFrame].top,
			m_rtVictoryFrames[m_iVictoryFrame].right,
			m_rtVictoryFrames[m_iVictoryFrame].bottom);
	}
	default:
		break;
	}
	if (m_CurrentView == HeroView::RightView)
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


void THeroObj::Render()
{
	Object2D::Render();
	Fade();
	m_pProjectile->Render(m_vCamera);
}

void THeroObj::Release()
{
	Object2D::Release();
	m_pProjectile->Release();
}

