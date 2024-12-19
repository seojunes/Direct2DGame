#include "THeroObj.h"
#include "TDevice.h"
int g_iJumpingCount = 0;
void THeroObj::Frame()
{
	float fSpeed = 100.0f;
	if (m_bIsJumping)
	{
		m_fVerticalSpeed -= m_fGravity * g_fSPF;
		m_srtScreen.y -= m_fVerticalSpeed * g_fSPF;
		if (m_srtScreen.y >= m_fGroundY)
		{
			m_srtScreen.y = m_fGroundY;
			m_bIsJumping = false;
			m_fVerticalSpeed = 0.0f;
			g_iJumpingCount = 0;
		}
	}

	//if (g_GameKey.dwWkey == KEY_HOLD) m_rtScreen.y -= fSpeed * g_fSPF;
	if (g_GameKey.dwSkey == KEY_HOLD) m_srtScreen.y += fSpeed * g_fSPF;
	if (g_GameKey.dwAkey == KEY_HOLD) m_srtScreen.x -= fSpeed * g_fSPF;
	if (g_GameKey.dwDkey == KEY_HOLD) m_srtScreen.x += fSpeed * g_fSPF;

	if (g_GameKey.dwWkey == KEY_PUSH && g_iJumpingCount < m_MaxJunp)//&& !m_bIsJumping)
	{
		m_bIsJumping = true;
		m_fVerticalSpeed = m_fJumpSpeed;
		g_iJumpingCount++;
	}
	// *시간의 동기화
	// 1000 frame = 1초 동안에 100이동하자.
				// 100 * 0.001f;
	// 100 frame  = 1초 동안에 100이동하자.
				// 100 * 0.01f;
	// 10 frame  = 1초 동안에 100이동하자.
				// 100 * 0.1f;
	/*float fSpeed = 100.0f;
	if (g_GameKey.dwWkey == KEY_HOLD)
	{
		m_srtScreen.y -= fSpeed * g_fSPF;
	}
	if (g_GameKey.dwSkey == KEY_HOLD)
	{
		m_srtScreen.y += fSpeed * g_fSPF;
	}
	if (g_GameKey.dwAkey == KEY_HOLD)
	{
		m_srtScreen.x -= fSpeed * g_fSPF;
	}
	if (g_GameKey.dwDkey == KEY_HOLD)
	{
		m_srtScreen.x += fSpeed * g_fSPF;
	}*/
	TVertex2 s = { m_srtScreen.x, m_srtScreen.y };
	// NDC <- Screen
	s.x = m_srtScreen.x / g_ptClientSize.x; // 0 ~1
	s.y = m_srtScreen.y / g_ptClientSize.y; // 0 ~1
	s.x = s.x * 2.0f - 1.0f;
	s.y = -(s.y * 2.0f - 1.0f);
	TVertex2 t;
	t.x = (m_srtScreen.x + m_srtScreen.w) / g_ptClientSize.x;
	t.y = (m_srtScreen.y + m_srtScreen.h) / g_ptClientSize.y;
	t.x = t.x * 2.0f - 1.0f;
	t.y = (t.y * 2.0f - 1.0f) * -1.0f;
	m_vVertexList[0].v = s;
	m_vVertexList[1].v = { t.x, s.y };
	m_vVertexList[2].v = { s.x, t.y };
	m_vVertexList[3].v = t;

	// gpu가 담당
	TDevice::m_pd3dContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
}
void THeroObj::SetVertexData()
{
	TObject::SetVertexData();
	float xSize = 669;
	float ySize = 1964;
	TRect rt;
	rt.SetS(2.0f, 128.0f, 39.0f, 170.0f);
	m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
	m_vVertexList[1].t = { rt.w / xSize,rt.y / ySize };
	m_vVertexList[2].t = { rt.x / xSize,rt.h / ySize };
	m_vVertexList[3].t = { rt.w / xSize,rt.h / ySize };

	/*rt.SetP(2.0f, 128.0f, 39.0f, 170.0f);
	m_vVertexList[0].t = { rt.x / xSize, rt.y / ySize };
	m_vVertexList[1].t = { rt.x2 / xSize, rt.y / ySize };
	m_vVertexList[2].t = { rt.x / xSize, rt.y2 / ySize };
	m_vVertexList[3].t = { rt.x2 / xSize, rt.y2 / ySize };*/
}