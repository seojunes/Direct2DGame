#include "THeroObj.h"
#include "TDevice.h"
#include "TWorld.h"

void    THeroObj::HitOverlap(TObject* pObj, THitResult hRes)
{

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

//void THeroObj::HandleCollision() {
//	m_CollisionDirection = CollisionDirection::None;
//
//	for (const auto& pair : m_pWorld->GetCollisionList()) {
//		TObject* rect = pair.second; // pair.second�� ���� TObject*��
//
//		if (rect->m_iCollisionType == TCollisionType::T_Block &&
//			TCollision::CheckRectToRect(this->m_rtScreen, rect->m_rtScreen)) {
//
//			// �浹 ���� ���
//			CollisionDirection dir = TCollision::DetectCollisionDirection(this->m_rtScreen, rect->m_rtScreen);
//
//			if (dir != CollisionDirection::None) {
//				m_CollisionDirection = dir; // �浹 ���� ����
//				break; // ù ��° �浹�� ó��
//			}
//		}
//	}
//}

void THeroObj::GetGroundH(float height)
{
	m_fGroundY = height;
}
void THeroObj::Frame()
{
	

	TVector2 vAdd;
	if (m_rtScreen.v2.y >= m_fGroundY)
	{
		m_CurrentState = HeroState::Idle;
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

	//if (g_GameKey.dwWkey == KEY_UP || !m_bIsJumping)
	//{
	//	//m_iJumpFrame = 0;
	//	//if (m_CurrentView == HeroView::RightView)
	//	//{
	//		m_CurrentState = HeroState::Idle; // RightRun ���� ����
	//	//}
	//	//else
	//	//{
	//	//	m_CurrentState = HeroState::LeftRun; // RightRun ���� ����
	//	//}

	//}


	// �ִϸ��̼� ������Ʈ
	if (g_GameKey.dwAkey == KEY_HOLD || g_GameKey.dwDkey == KEY_HOLD)
	{
		m_fCurrentTime += g_fSPF; // �ð� ������Ʈ
		if (m_fCurrentTime >= m_fFrameTime)
		{
			m_fCurrentTime -= m_fFrameTime;
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
	//else if (g_GameKey.dwWkey == KEY_HOLD)// && m_bIsJumping
	else if(m_CurrentState == HeroState::Jump)
	{
		m_fCurrentTime += g_fSPF; // �ð� ������Ʈ
		if (m_fCurrentTime >= m_fFrameTime)
		{
			m_fCurrentTime -= m_fFrameTime;
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
	else
	{
		m_iWalkFrame = 0; // ���� ���¿����� ù ������ ����
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

	

	AddPosition(vAdd);
	SetVertexData();

	if (m_bCharging == false)
	{
		m_fAlpha = 1.0f;
	}

	if (g_GameKey.dwSpace == KEY_PUSH)
	{
		// �ʱ�ȭ: Ű�� ������ �����ϸ� ���� �ð� ����
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
			m_pProjectile->AddEffect(vStart, vEnd, 1, m_bOnCharing);
		}
		else
		{
			m_pProjectile->AddEffect(vStart, vEnd, 2, m_bOnCharing);
		}
		// ���� �ð� �ʱ�ȭ
		m_fChargingTime = 0.0f;
	}
	/*if (m_bCharging == true)  // ������ ȿ��
	{
		if(m_fAlpha <= 1.0f)
		m_fAlpha = cosf(g_fGT * 5.0f) * 0.25f + 0.75f;
	}*/
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
		m_rtWalkFrames[m_iWalkFrame].bottom); // ���� �������� �ؽ�ó ��ǥ,

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

	//�¿�ȱ�
	//if (m_CurrentState == RightRun)
	//{
	//	m_vVertexList[0].t = { rt.x / xSize, rt.y / ySize };
	//	m_vVertexList[1].t = { rt.w / xSize, rt.y / ySize };
	//	m_vVertexList[2].t = { rt.x / xSize, rt.h / ySize };
	//	m_vVertexList[3].t = { rt.w / xSize, rt.h / ySize };
	//}
	//// LeftRun ���� (UV �¿� ����)
	//else if (m_CurrentState == LeftRun)
	//{
	//	// ������ ��ǥ
	//	m_vVertexList[0].t = { rt.w / xSize, rt.y / ySize }; 
	//	m_vVertexList[1].t = { rt.x / xSize, rt.y / ySize };
	//	m_vVertexList[2].t = { rt.w / xSize, rt.h / ySize };
	//	m_vVertexList[3].t = { rt.x / xSize, rt.h / ySize };
	//}


	/*rt.SetS(2.0f, 128.0f, 39.0f, 170.0f);
	m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
	m_vVertexList[1].t = { rt.w / xSize,rt.y / ySize };
	m_vVertexList[2].t = { rt.x / xSize,rt.h / ySize };
	m_vVertexList[3].t = { rt.w / xSize,rt.h / ySize };*/

	/*rt.SetP(2.0f, 128.0f, 39.0f, 170.0f);
	m_vVertexList[0].t = { rt.x / xSize, rt.y / ySize };
	m_vVertexList[1].t = { rt.x2 / xSize, rt.y / ySize };
	m_vVertexList[2].t = { rt.x / xSize, rt.y2 / ySize };
	m_vVertexList[3].t = { rt.x2 / xSize, rt.y2 / ySize };*/

}


//void THeroObj::Render() 
//{
//	if (isInvincible) {
//		blinkTimer += g_fGT;
//
//		// ������ ȿ�� (0.2�� ����)
//		if (fmod(blinkTimer, 0.2f) < 0.1f) {
//			return; // ������ �ֱ� ���� ������ ����
//		}
//	}
//
//	// �⺻ TObject ������ ȣ��
//	TObject::Render();
//}
//
//void THeroObj::Update(float g_fGT)
//{
//	if (isInvincible) {
//		invincibleTime -= g_fGT; // ���� �ð� ����
//		if (invincibleTime <= 0.0f) {
//			isInvincible = false; // ���� ���� ����
//			blinkTimer = 0.0f;    // ������ Ÿ�̸� �ʱ�ȭ
//		}
//	}
//
//	// TObject�� �⺻ ������Ʈ ȣ�� (�ʿ��ϸ�)
//	TObject::Frame();
//}

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

