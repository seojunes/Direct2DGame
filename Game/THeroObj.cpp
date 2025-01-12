#include "THeroObj.h"
#include "TDevice.h"
#include "TWorld.h"

void    THeroObj::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();
	if (OtherType == TObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile && pMissile->m_pOwner != Shooter::OWNER_HERO)
		{
			m_HP -= pMissile->m_Data.m_iDamage;
			pMissile->m_bDead = true;
		}
	}
	
	if (OtherType == TObjectType::Portal)
	{
		m_BossMoving = BossRoomMovingState::STATE_ABLE;
	}
	else
	{
		m_BossMoving = BossRoomMovingState::STATE_UNABLE;
	}
	if (OtherType == TObjectType::Npc)
	{
		TakeDamage(5);
	}
}

void THeroObj::Init()
{
	TObject2D::Init();
	m_pProjectile = std::make_shared<TProjectile>(m_pWorld);
}

void THeroObj::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtWalkFrames.resize(SpriteList[0].size());
	m_rtJumpFrames.resize(SpriteList[1].size());
	m_rtWalkFrames = SpriteList[0];
	m_rtJumpFrames = SpriteList[1];
}

void THeroObj::TakeDamage(int damage)
{
	if (m_bInvincible == false) // 무적 상태가 아닐 때만 데미지를 받음
	{
		m_HP -= damage;

		// 무적 상태 시작
		m_bInvincible = true;
		m_fInvincibleTime = m_fMaxInvincibleTime;
	}

}

void THeroObj::GetGroundH(float height)
{
	m_fGroundY = height;
}
void THeroObj::Frame()
{

	if (m_HP <= 0)
	{
		m_HP = 0;
		m_bDead = true; // 히어로 사망 처리
	}

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
	if (m_rtScreen.v2.y >= m_fGroundY)
	{
		m_CurrentState = HeroState::Idle;
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
		}
	}

	// 애니메이션 업데이트
	if (g_GameKey.dwAkey == KEY_HOLD || g_GameKey.dwDkey == KEY_HOLD)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fFrameTime)
		{
			m_fCurrentTime -= m_fFrameTime;
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
	//else if (g_GameKey.dwWkey == KEY_HOLD)// && m_bIsJumping
	else if (m_CurrentState == HeroState::Jump)
	{
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fFrameTime)
		{
			m_fCurrentTime -= m_fFrameTime;
			m_iJumpFrame++; // 다음 프레임으로 이동
			if (m_iJumpFrame >= m_rtJumpFrames.size())
			{
				if (m_bLoop)
					m_iJumpFrame = 0; // 반복일 경우 첫 프레임으로 이동
				else
					m_iJumpFrame = m_rtJumpFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	else
	{
		m_iWalkFrame = 0; // 정지 상태에서는 첫 프레임 유지
	}

	if (g_GameKey.dwWkey == KEY_PUSH && m_iJumpingCount < m_MaxJunp)//&& !m_bIsJumping)
	{
		m_fGroundY = 1800.0f;
		m_bIsJumping = true;
		m_fVerticalSpeed = m_fJumpSpeed;
		m_iJumpingCount++;
		m_CurrentState = HeroState::Jump; // Jump 상태 설정.
	}
	if (g_GameKey.dwAkey == KEY_HOLD)
	{
		m_fGroundY = 1800.0f;
		vAdd.x -= m_fSpeed * g_fSPF;
		m_CurrentState = HeroState::LeftRun;
		m_CurrentView = HeroView::LeftView;
	}
	if (g_GameKey.dwDkey == KEY_HOLD)
	{
		m_fGroundY = 1800.0f;
		vAdd.x += m_fSpeed * g_fSPF;
		m_CurrentState = HeroState::RightRun;
		m_CurrentView = HeroView::RightView;
	}
	if (m_bCharging == false)
	{
		m_fAlpha = 1.0f;
	}

	if (g_GameKey.dwSpace == KEY_PUSH)
	{
		// 초기화: 키를 누르기 시작하면 충전 시간 리셋
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
			m_pProjectile->AddEffect(vStart, vEnd, m_vRightDir, Shooter::OWNER_HERO, m_bOnCharing);
		}
		else
		{
			m_pProjectile->AddEffect(vStart, vEnd, m_vLeftDir, Shooter::OWNER_HERO, m_bOnCharing);
		}
		// 충전 시간 초기화
		//m_pProjectile->
		m_fChargingTime = 0.0f;
	}
	/*if (m_bCharging == true)  // 깜빡임 효과
	{
		if(m_fAlpha <= 1.0f)
		m_fAlpha = cosf(g_fGT * 5.0f) * 0.25f + 0.75f;
	}*/

	AddPosition(vAdd);
	SetVertexData();


	m_pProjectile->Frame(m_vPos);

}
void THeroObj::SetVertexData()
{
	if (m_pTexture == nullptr || m_rtWalkFrames.empty()) return;
	TObject2D::SetVertexData();

	TRect rt;
	rt.SetS(m_rtWalkFrames[m_iWalkFrame].left,
		m_rtWalkFrames[m_iWalkFrame].top,
		m_rtWalkFrames[m_iWalkFrame].right,
		m_rtWalkFrames[m_iWalkFrame].bottom); // 현재 프레임의 텍스처 좌표,

	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;

	if (m_CurrentState == HeroState::Idle)
	{
		rt.SetS(0.0f, 0.0f, 50.0f, 50.0f);
		if (m_CurrentView == RightView)
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
	switch (m_CurrentState)
	{
	case HeroState::RightRun:
		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		break;
	case HeroState::LeftRun:
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		break;
	case HeroState::Jump:
		rt.SetS(m_rtJumpFrames[m_iJumpFrame].left,
			m_rtJumpFrames[m_iJumpFrame].top,
			m_rtJumpFrames[m_iJumpFrame].right,
			m_rtJumpFrames[m_iJumpFrame].bottom);
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
		break;
	default:
		break;
	}


}


void THeroObj::Render()
{
	TObject2D::Render();
	m_pProjectile->Render(m_vCamera);
}

void THeroObj::Release()
{
	TObject2D::Release();
	m_pProjectile->Release();
}

