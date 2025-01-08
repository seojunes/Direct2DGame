#include "TMonster.h"

void TMonster1::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(8.0f, 0.0f, 35.0f, 26.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TMonster1::Frame()
{
	if (m_state == Monster1State::STATE_Move)
	{
		//if (m_vPos.x <= m_vInitedPos.x + m_iLimitedDis && m_vPos.x >= m_vInitedPos.x - m_iLimitedDis)
		
		TVector2 vMove = m_vPos + m_vDir * (g_fSPF * m_fSpeed);
		SetPosition(vMove);
		if (m_vPos.x >= m_vInitedPos.x + m_iLimitedDis || m_vPos.x <= m_vInitedPos.x - m_iLimitedDis)
		{
			m_vDir.x *= -1.0f;
		}
		
	}
}

void TMonster2::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(3.0f, 7.0f, 27.0f, 36.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TMonster2::Frame()
{

}

void TMonster3::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(10.0f, 26.0f, 57.0f, 57.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TMonster3::Frame()
{

}
