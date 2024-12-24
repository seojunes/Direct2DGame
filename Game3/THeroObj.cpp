#include "THeroObj.h"
#include "TDevice.h"

void THeroObj::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtWalkFrames.resize(SpriteList[0].size());
	m_rtJumpFrames.resize(SpriteList[1].size());
	m_rtWalkFrames = SpriteList[0];
	m_rtJumpFrames = SpriteList[1];
}


void THeroObj::Frame()
{
	// 점프효과 구현
	if (m_bIsJumping)
	{
		m_fVerticalSpeed -= m_fGravity * g_fSPF;
		m_vPos.y -= m_fVerticalSpeed * g_fSPF;
		if (m_vPos.y >= m_fGroundY)
		{
			m_vPos.y = m_fGroundY;
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
	//if (g_GameKey.dwWkey == KEY_HOLD) m_rtScreen.y -= fSpeed * g_fSPF;
	/*if (g_GameKey.dwSkey == KEY_HOLD)
	{
		m_vPos.y += m_fSpeed * g_fSPF;
	}*/
	if (g_GameKey.dwAkey == KEY_HOLD)
	{
		m_vPos.x -= m_fSpeed * g_fSPF;
		m_CurrentState = HeroState::LeftRun; // LeftRun 상태 설정
		m_CurrentView = HeroView::LeftView;  // 바라보는 방향 설정
	}
	if (g_GameKey.dwDkey == KEY_HOLD)
	{
		m_vPos.x += m_fSpeed * g_fSPF;
		m_CurrentState = HeroState::RightRun; // RightRun 상태 설정
		m_CurrentView = HeroView::RightView;  // 바라보는 방향 설정
	}


	UpdatePosition();
	SetVertexData(); // UV 반영을 위해 호출
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
		m_rtWalkFrames[m_iWalkFrame].bottom); // 현재 프레임의 텍스처 좌표

	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	// 기준 크기
	float standardWidth = 80.0f;
	float standardHeight = 100.0f;

	if (m_CurrentState == HeroState::RightRun)
	{
		m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
		m_vVertexList[1].t = { (rt.x + rt.w) / xSize,rt.y / ySize };
		m_vVertexList[2].t = { rt.x / xSize,(rt.y + rt.h) / ySize };
		m_vVertexList[3].t = { (rt.x + rt.w) / xSize,(rt.y + rt.h) / ySize };
	}
	else if (m_CurrentState == HeroState::LeftRun)
	{
		m_vVertexList[0].t = { (rt.x + rt.w) / xSize,rt.y / ySize };
		m_vVertexList[1].t = { rt.x / xSize,rt.y / ySize };
		m_vVertexList[2].t = { (rt.x + rt.w) / xSize,(rt.y + rt.h) / ySize };
		m_vVertexList[3].t = { rt.x / xSize,(rt.y + rt.h) / ySize };
	}

	if (m_CurrentState == HeroState::Jump )
	{
		rt.SetS(m_rtJumpFrames[m_iJumpFrame].left,
			m_rtJumpFrames[m_iJumpFrame].top,
			m_rtJumpFrames[m_iJumpFrame].right,
			m_rtJumpFrames[m_iJumpFrame].bottom);
		if (m_CurrentView == HeroView::RightView)
		{
			m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
			m_vVertexList[1].t = { (rt.x + rt.w) / xSize,rt.y / ySize };
			m_vVertexList[2].t = { rt.x / xSize,(rt.y + rt.h) / ySize };
			m_vVertexList[3].t = { (rt.x + rt.w) / xSize,(rt.y + rt.h) / ySize };
		}
		else
		{
			m_vVertexList[0].t = {(rt.x + rt.w) / xSize,rt.y / ySize };
			m_vVertexList[1].t = { rt.x / xSize,rt.y / ySize };
			m_vVertexList[2].t = { (rt.x + rt.w) / xSize,(rt.y + rt.h) / ySize };
			m_vVertexList[3].t = { rt.x / xSize,(rt.y + rt.h) / ySize };
		}
	}
	m_vVertexList[0].v = { m_srtScreen.x - rt.w / 2.0f, m_srtScreen.y - rt.h };
	m_vVertexList[1].v = { m_srtScreen.x + rt.w / 2.0f, m_srtScreen.y - rt.h };
	m_vVertexList[2].v = { m_srtScreen.x - rt.w / 2.0f, m_srtScreen.y };
	m_vVertexList[3].v = { m_srtScreen.x + rt.w / 2.0f, m_srtScreen.y };

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