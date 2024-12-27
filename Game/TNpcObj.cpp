#include "TNpcObj.h"
#include "TDevice.h"
void TNpcObj::Frame()
{
	if (m_vPos.x > m_pMap->m_srtScreen.x2 - m_srtScreen.w)
	{
		m_vDir.x *= -1.0f;
		m_vPos.x = m_pMap->m_srtScreen.x2 - m_srtScreen.w;
	}
	if (m_vPos.x < 0.0f)
	{
		m_vDir.x *= -1.0f;
		m_vPos.x = 0.0f;
	}
	if (m_vPos.y > m_pMap->m_srtScreen.y2 - m_srtScreen.h)
	{
		m_vDir.y *= -1.0f;
		m_vPos.y = m_pMap->m_srtScreen.y2 - m_srtScreen.h;
	}
	if (m_vPos.y < 0.0f)
	{
		m_vDir.y *= -1.0f;
		m_vPos.y = 0.0f;
	}
	// v = v + d*s : 직선의 벡터의 방정식
	m_vPos = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
	SetPos(m_vPos);
}
void TNpcObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	TObject2D::SetVertexData();
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetS(46.0f, 62.0f, 114.0f, 142.0f);
	m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
	m_vVertexList[1].t = { rt.w / xSize,rt.y / ySize };
	m_vVertexList[2].t = { rt.x / xSize,rt.h / ySize };
	m_vVertexList[3].t = { rt.w / xSize,rt.h / ySize };
}