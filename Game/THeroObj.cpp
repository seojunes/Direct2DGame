#include "THeroObj.h"
#include "TDevice.h"
#include "TWorld.h"

void    THeroObj::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();
	//Hero�� �� �̻����� �ƴ� �̻��Ͽ� ������ �̻��ϵ�������ŭ �� ����
	if (OtherType == TObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile && pMissile->m_pOwnerType != Shooter::OWNER_HERO && !m_bInvincible && pMissile->m_Data.m_pOwner->m_bDead ==false)
		{
			TakeDamage(pMissile->m_Data.m_iDamage);
			pMissile->m_bDead = true;
		}
	}
	//��Ż�� ������ ���������� �̵�����
	if (OtherType == TObjectType::Portal)
	{
		m_BossMoving = BossRoomMovingState::STATE_ABLE;
	}
	else
	{
		m_BossMoving = BossRoomMovingState::STATE_UNABLE;
	}

	// ���Ϳ� �ε�������, ������
	if (OtherType == TObjectType::Npc)
	{
		TakeDamage(5);
	}
	else if (OtherType == TObjectType::Boss)
	{
		TakeDamage(20);
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
	m_rtIdleFrames.resize(SpriteList[2].size());
	m_rtShotFrames.resize(SpriteList[3].size());
	m_rtShotNRunFrames.resize(SpriteList[4].size());
	m_rtVictoryFrames.resize(SpriteList[5].size());

	m_rtWalkFrames = SpriteList[0];
	m_rtJumpFrames = SpriteList[1];
	m_rtIdleFrames = SpriteList[2];
	m_rtShotFrames = SpriteList[3];
	m_rtShotNRunFrames = SpriteList[4];
	m_rtVictoryFrames = SpriteList[5];
}

void THeroObj::TakeDamage(int damage)
{
	if (m_bInvincible == false) // ���� ���°� �ƴ� ���� �������� ����
	{
		m_HP -= damage;

		// ���� ���� ����
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
		m_bDead = true; // ����� ��� ó��
	}

	if (m_bInvincible)
	{
		m_fInvincibleTime -= g_fSPF;
		if (m_fInvincibleTime <= 0.0f)
		{
			m_bInvincible = false; // ���� ���� ����
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
	
	// ����ȿ�� ����
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
	// Ű �Է´� 
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

			if (g_GameKey.dwSpace == KEY_PUSH || g_GameKey.dwSpace == KEY_HOLD)
			{
				m_fChargingTime = 0.0f;
				//m_bIsShooting = true;
			}
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

			if (g_GameKey.dwSpace == KEY_PUSH || g_GameKey.dwSpace == KEY_HOLD)
			{
				m_fChargingTime = 0.0f;
				//m_bIsShooting = true;
			}
		}
		if (g_GameKey.dwWkey == KEY_PUSH && m_iJumpingCount < m_MaxJunp)//&& !m_bIsJumping)
		{
			m_fGroundY = 1800.0f;
			m_bIsJumping = true;
			m_fVerticalSpeed = m_fJumpSpeed;
			m_iJumpingCount++;
			m_CurrentState = HeroState::Jump; // Jump ���� ����.
		}
		if (g_GameKey.dwSpace == KEY_PUSH)
		{
			m_fChargingTime = 0.0f;
		}


		if (g_GameKey.dwSpace == KEY_HOLD)
		{
			// Ű�� ������ �ִ� ���� ���� �ð� ����
			m_fChargingTime += g_fSPF;
			m_bCharging = true;
		}

		if (g_GameKey.dwSpace == KEY_UP)
		{
			m_bCharging = false;
			// Ű�� ���� ���� �߻�
			TVector2 vHalf;
			TVector2 vStart;
			TVector2 vEnd;

			if (m_fChargingTime < 0.5f)
			{
				// �Ϲ� �̻���
				vHalf = { 20.0f, 20.0f };
				vStart = m_vPos - vHalf;
				vEnd = m_vPos + vHalf;
				m_bOnCharing = false;
			}
			else
			{
				// ��¡��
				vHalf = { 25.0f, 40.0f };
				vStart = m_vPos - vHalf;
				vEnd = m_vPos + vHalf;
				m_bOnCharing = true;
			}

			if (m_CurrentView == RightView)								//�ٶ󺸴� ���⿡ ���� �߻�ü ����.
			{
				m_pProjectile->AddEffect(vStart, vEnd, m_vRightDir, Shooter::OWNER_HERO, this, m_bOnCharing);
			}
			else
			{
				m_pProjectile->AddEffect(vStart, vEnd, m_vLeftDir, Shooter::OWNER_HERO, this, m_bOnCharing);
			}
			// ���� �ð� �ʱ�ȭ
			//m_pProjectile->
			m_fChargingTime = 0.0f;
			m_bIsShooting = true;
			m_fShootingMotionTime = m_fMaxMotionTime;
		}
	}
	
	
	// �ִϸ��̼� ������Ʈ
	if (m_CurrentState == HeroState::Idle)
	{
		m_fCurrentTime += g_fSPF; // �ð� ������Ʈ
		if (m_fCurrentTime >= m_fIdleFrameTime)
		{
			m_fCurrentTime -= m_fIdleFrameTime;
			m_iIdleFrame++; // ���� ���������� �̵�
			if (m_iIdleFrame >= m_rtIdleFrames.size())
			{
				if (m_bLoop)
					m_iIdleFrame = 0; // �ݺ��� ��� ù ���������� �̵�
				else
					m_iIdleFrame = m_rtIdleFrames.size() - 1; // ������ ������ ����
			}
		}
	}
	if ((g_GameKey.dwAkey == KEY_HOLD || g_GameKey.dwDkey == KEY_HOLD) && m_CurrentState != HeroState::Jump)
	{
		m_fCurrentTime += g_fSPF; // �ð� ������Ʈ
		if (m_fCurrentTime >= m_fWalkFrameTime)
		{
			m_fCurrentTime -= m_fWalkFrameTime;
			m_iWalkFrame++; // ���� ���������� �̵�
			if (m_iWalkFrame >= m_rtWalkFrames.size())
			{
				if (m_bLoop)
					m_iWalkFrame = 0; // �ݺ��� ��� ù ���������� �̵�
				else
					m_iWalkFrame = m_rtWalkFrames.size() - 1; // ������ ������ ����
			}
		}
	}
	if (m_CurrentState == HeroState::Jump)
	{
		m_fCurrentTime += g_fSPF; // �ð� ������Ʈ
		if (m_fCurrentTime >= m_fJumpFrameTime)
		{
			m_fCurrentTime -= m_fJumpFrameTime;
			m_iJumpFrame++; // ���� ���������� �̵�
			if (m_iJumpFrame >= m_rtJumpFrames.size())
			{
				if (m_bLoop)
					m_iJumpFrame = 0; // �ݺ��� ��� ù ���������� �̵�
				else
					m_iJumpFrame = m_rtJumpFrames.size() - 1; // ������ ������ ����
			}
		}
	}
	if (m_CurrentState == HeroState::Victory)
	{
		m_fCurrentTime += g_fSPF; // �ð� ������Ʈ
		if (m_fCurrentTime >= m_fVictoryFrameTime)
		{
			m_fCurrentTime -= m_fVictoryFrameTime;
			m_iVictoryFrame++; // ���� ���������� �̵�
			if (m_iVictoryFrame >= m_rtVictoryFrames.size())
			{
				if (m_bLoop)
					m_iVictoryFrame = 0; // �ݺ��� ��� ù ���������� �̵�
				else
					m_iVictoryFrame = m_rtVictoryFrames.size() - 1; // ������ ������ ����
			}
		}
	}

	if (m_bCharging == false)
	{
		m_fAlpha = 1.0f;
	}

	
	if (m_bInvincible == true)  // ������ ȿ��
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
	if (m_pTexture == nullptr || m_rtWalkFrames.empty()) return;
	TObject2D::SetVertexData();

	TRect rt;


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
				m_rtWalkFrames[m_iWalkFrame].bottom); // ���� �������� �ؽ�ó ��ǥ,
			break;
		}
		else
		{
			rt.SetS(m_rtShotNRunFrames[m_iWalkFrame].left,
				m_rtShotNRunFrames[m_iWalkFrame].top,
				m_rtShotNRunFrames[m_iWalkFrame].right,
				m_rtShotNRunFrames[m_iWalkFrame].bottom); // ���� �������� �ؽ�ó ��ǥ,
			break;
		}
	case HeroState::Jump:
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
			rt.SetS(m_rtShotFrames[0].left,
				m_rtShotFrames[0].top,
				m_rtShotFrames[0].right,
				m_rtShotFrames[0].bottom);
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
	TObject2D::Render();
	Fade();
	m_pProjectile->Render(m_vCamera);
}

void THeroObj::Release()
{
	TObject2D::Release();
	m_pProjectile->Release();
}

