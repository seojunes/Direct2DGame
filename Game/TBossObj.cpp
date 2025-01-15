#include "TBossObj.h"
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
		switch (m_iBossAttack1Frame)
		{
		case 0:
			SetScale({ 73,36 });
			break;
		case 1:
			SetScale({ 73,31 });
			break;
		case 2:
			SetScale({ 54,35 });
			break;
		case 3:
			SetScale({ 34,35 });
			break;
		default:
			break;
		}
		break;
	case BossState::STATE_Attack2:
		break;
	case BossState::STATE_Attack3:
		break;
	case BossState::STATE_Return:
		break;
	default:
		break;
	}
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	/*rt.SetP(0.0f, 0.0f, 63.0f, 57.0f);*/
	/*m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };*/

	m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };


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
				m_iBossAttack1Frame = m_rtBossAttack1Frames.size() - 1; // 마지막 프레임 유지
			}
		}
	}
	
	

	/*TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(vMove);*/
	float fHeroDistance = (m_pHero->m_vPos - m_vPos).Length();
	float fInitDistance = (m_vInitedPos - m_vPos).Length();
	if (m_state == BossState::STATE_Create)
	{
		m_fCreateTime -= g_fSPF;
		if (m_fCreateTime <= 0.0f)
		{
			m_state = BossState::STATE_Idle;
		}
	}
	else if (m_state == BossState::STATE_Idle)
	{
		if (fHeroDistance<150.0f || m_iPreHP != m_HP)
		{
			m_state = BossState::STATE_Attack1;
		}
	}
	//else if (m_state == BossState::STATE_Move)
	//{
	//	//if (m_vPos.x <= m_vInitedPos.x + m_iLimitedDis && m_vPos.x >= m_vInitedPos.x - m_iLimitedDis)
	//	m_fSpeed = 350.0f;
	//	m_vDir = (m_vMapCenter - m_vPos).Normal();

	//	if ((m_vMapCenter - m_vPos).Length() < 1.0f)
	//	{
	//		m_state = BossState::STATE_Attack1;
	//	}
	//}
	else if (m_state == BossState::STATE_Attack1)
	{
		//m_fSpeed = 100.0f;
		//m_vDir = { 0.0f, 1.0f };
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