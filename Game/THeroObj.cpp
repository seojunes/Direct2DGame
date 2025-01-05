#include "THeroObj.h"
#include "TDevice.h"
#include "TWorld.h"

void    THeroObj::HitOverlap(TObject* pObj, THitResult hRes)
{

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
//		TObject* rect = pair.second; // pair.second가 실제 TObject*임
//
//		if (rect->m_iCollisionType == TCollisionType::T_Block &&
//			TCollision::CheckRectToRect(this->m_rtScreen, rect->m_rtScreen)) {
//
//			// 충돌 방향 계산
//			CollisionDirection dir = TCollision::DetectCollisionDirection(this->m_rtScreen, rect->m_rtScreen);
//
//			if (dir != CollisionDirection::None) {
//				m_CollisionDirection = dir; // 충돌 방향 저장
//				break; // 첫 번째 충돌만 처리
//			}
//		}
//	}
//}

void THeroObj::Frame()
{

	TVector2 vAdd;

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



	if (g_GameKey.dwWkey == KEY_PUSH && m_iJumpingCount < m_MaxJunp)//&& !m_bIsJumping)
	{
		m_bIsJumping = true;
		m_fVerticalSpeed = m_fJumpSpeed;
		m_iJumpingCount++;
		m_CurrentState = HeroState::Jump; // Jump 상태 설정.
	}

	if (g_GameKey.dwWkey == KEY_UP || !m_bIsJumping)
	{
		m_iJumpFrame = 0;
		if (m_CurrentView == HeroView::RightView)
		{
			m_CurrentState = HeroState::RightRun; // RightRun 상태 설정
		}
		else
		{
			m_CurrentState = HeroState::LeftRun; // RightRun 상태 설정
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
	else if (g_GameKey.dwWkey == KEY_HOLD && m_bIsJumping)
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



	////if (g_GameKey.dwWkey == KEY_HOLD) m_rtScreen.y -= fSpeed * g_fSPF;
	//if (g_GameKey.dwAkey == KEY_HOLD)
	//{
	//	vAdd.x -= m_fSpeed * g_fSPF;
	//	m_CurrentState = HeroState::LeftRun; // LeftRun 상태 설정
	//	m_CurrentView = HeroView::LeftView;  // 바라보는 방향 설정
	//}
	//if (g_GameKey.dwDkey == KEY_HOLD)
	//{
	//	vAdd.x += m_fSpeed * g_fSPF;
	//	m_CurrentState = HeroState::RightRun; // RightRun 상태 설정
	//	m_CurrentView = HeroView::RightView;  // 바라보는 방향 설정
	//}

	//if (g_GameKey.dwSkey == KEY_HOLD)
	//{
	//	m_vPos.y += m_fSpeed * g_fSPF;
	//}

	//if (g_GameKey.dwWkey == KEY_HOLD)
	//{
	//	m_vPos.y -= m_fSpeed * g_fSPF;
	//	m_CurrentState = HeroState::RightRun; // RightRun 상태 설정
	//	m_CurrentView = HeroView::RightView;  // 바라보는 방향 설정
	//}
	if (g_GameKey.dwAkey == KEY_HOLD && m_CollisionDirection != Left)
	{
		vAdd.x -= m_fSpeed * g_fSPF;
		m_CurrentState = HeroState::LeftRun;
		m_CurrentView = HeroView::LeftView;
	}
	if (g_GameKey.dwDkey == KEY_HOLD && m_CollisionDirection != Right)
	{
		vAdd.x += m_fSpeed * g_fSPF;
		m_CurrentState = HeroState::RightRun;
		m_CurrentView = HeroView::RightView;
	}

	// 위아래 이동
	/*if (g_GameKey.dwWkey == KEY_HOLD && m_CollisionDirection != Top)
	{
		vAdd.y -= m_fSpeed * g_fSPF;
	}
	if (g_GameKey.dwSkey == KEY_HOLD && m_CollisionDirection != Bottom)
	{
		vAdd.y += m_fSpeed * g_fSPF;
	}*/
	AddPosition(vAdd);
	SetVertexData();
}
void THeroObj::SetVertexData()
{
	if (m_pTexture == nullptr || m_rtWalkFrames.empty()) return;
	TObject2D::SetVertexData();
	/*float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetS(2.0f, 128.0f, 39.0f, 170.0f);*/

	TRect rt;
	rt.SetS(m_rtWalkFrames[m_iWalkFrame].left,
		m_rtWalkFrames[m_iWalkFrame].top,
		m_rtWalkFrames[m_iWalkFrame].right,
		m_rtWalkFrames[m_iWalkFrame].bottom); // 현재 프레임의 텍스처 좌표,

	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;

	if (m_CurrentState == HeroState::RightRun)
	{
		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	}
	else if (m_CurrentState == HeroState::LeftRun)
	{
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	}

	if (m_CurrentState == HeroState::Jump)
	{
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
	}



	//좌우걷기
	//if (m_CurrentState == RightRun)
	//{
	//	m_vVertexList[0].t = { rt.x / xSize, rt.y / ySize };
	//	m_vVertexList[1].t = { rt.w / xSize, rt.y / ySize };
	//	m_vVertexList[2].t = { rt.x / xSize, rt.h / ySize };
	//	m_vVertexList[3].t = { rt.w / xSize, rt.h / ySize };
	//}
	//// LeftRun 상태 (UV 좌우 반전)
	//else if (m_CurrentState == LeftRun)
	//{
	//	// 반전된 좌표
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
//		// 깜빡임 효과 (0.2초 간격)
//		if (fmod(blinkTimer, 0.2f) < 0.1f) {
//			return; // 깜빡임 주기 동안 렌더링 생략
//		}
//	}
//
//	// 기본 TObject 렌더링 호출
//	TObject::Render();
//}
//
//void THeroObj::Update(float g_fGT)
//{
//	if (isInvincible) {
//		invincibleTime -= g_fGT; // 무적 시간 감소
//		if (invincibleTime <= 0.0f) {
//			isInvincible = false; // 무적 상태 해제
//			blinkTimer = 0.0f;    // 깜빡임 타이머 초기화
//		}
//	}
//
//	// TObject의 기본 업데이트 호출 (필요하면)
//	TObject::Frame();
//}


