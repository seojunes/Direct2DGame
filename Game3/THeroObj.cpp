#include "THeroObj.h"
#include "TDevice.h"

void THeroObj::Frame()
{
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

	//if (g_GameKey.dwWkey == KEY_HOLD) m_rtScreen.y -= fSpeed * g_fSPF;
	if (g_GameKey.dwSkey == KEY_HOLD)  m_vPos.y += m_fSpeed * g_fSPF;
	if (g_GameKey.dwAkey == KEY_HOLD) m_vPos.x -= m_fSpeed * g_fSPF;
	if (g_GameKey.dwDkey == KEY_HOLD) m_vPos.x += m_fSpeed * g_fSPF;

	if (g_GameKey.dwWkey == KEY_PUSH && m_iJumpingCount < m_MaxJunp)//&& !m_bIsJumping)
	{
		m_bIsJumping = true;
		m_fVerticalSpeed = m_fJumpSpeed;
		m_iJumpingCount++;
	}
	UpdatePosition();
}
void THeroObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	TObject2D::SetVertexData();
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
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