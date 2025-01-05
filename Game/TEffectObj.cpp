#include "TEffectObj.h"
#include "TDevice.h"
void TEffectObj::SetData(TEffectData data)
{
	m_Data = data;
	m_rtList = m_Data.m_rtList;
	m_iNumAnimFrame = m_Data.m_iNumAnimFrame;
	m_iAnimFrame = 0;
	m_fLifeTime = m_Data.m_fLifeTime;
	m_fOffsetTime = m_fLifeTime / m_iNumAnimFrame;
	m_bLoop = m_Data.m_bLoop;
	m_bDead = false;
}
void TEffectObj::Frame()
{
	/*TVector2 s = ScreenToNDC(m_rtScreen.x, m_rtScreen.y,
		g_ptClientSize);
	TVector2 e = ScreenToNDC(m_rtScreen.x2,m_rtScreen.y2,
		g_ptClientSize);
	if (m_Data.m_iType == 0)
	{
		e = ScreenToNDC(m_rtScreen.x + m_rtList[m_iAnimFrame].right,
						m_rtScreen.y + m_rtList[m_iAnimFrame].bottom, g_ptClientSize);
	}
	m_vVertexList[0].v = s;
	m_vVertexList[1].v = { e.x, s.y };
	m_vVertexList[2].v = { s.x, e.y };
	m_vVertexList[3].v = e;	*/

	m_fCurrentTime += g_fSPF;
	m_fLifeTime -= g_fSPF;
	if (m_fLifeTime <= 0.0f) m_bDead = true;
	if (m_fCurrentTime > m_fOffsetTime)
	{
		m_iAnimFrame++;
		m_fCurrentTime -= m_fOffsetTime;
		if (m_Data.m_iType == 0)
		{
			if (m_iAnimFrame >= m_rtList.size())
			{
				m_iAnimFrame = 0;
				if (m_bLoop == false)
				{
					m_bDead = true;
				}
			}
		}
		if (m_Data.m_iType == 1)
		{
			if (m_iAnimFrame >= m_Data.m_szList.size())
			{
				m_iAnimFrame = 0;
				if (m_bLoop == false)
				{
					m_bDead = true;
				}
			}
		}

	}
	if (m_Data.m_iType == 0)
	{
		TRect rt;
		rt.SetS(m_rtList[m_iAnimFrame].left,
			m_rtList[m_iAnimFrame].top,
			m_rtList[m_iAnimFrame].right,
			m_rtList[m_iAnimFrame].bottom);

		if (m_pTexture)
		{
			float xSize = m_pTexture->m_TexDesc.Width;
			float ySize = m_pTexture->m_TexDesc.Height;
			m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
			m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
			m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
			m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		}
	}
	if (m_Data.m_iType == 1)
	{
		m_pCurrentTexture = I_Tex.GetPtr(m_Data.m_szList[m_iAnimFrame]);
	}
}
void TEffectObj::Render()
{
	TDevice::m_pd3dContext->UpdateSubresource(
		m_pMeshRender->m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
	// 텍스처 교체 스프라이트
	if (m_Data.m_iType == 1)
	{
		PreRender();
		TDevice::m_pd3dContext->PSSetShaderResources(
			0, 1, &m_pCurrentTexture->m_pTexSRV);
		if (m_pShader)
		{
			TDevice::m_pd3dContext->VSSetShader(
				m_pShader->m_pVertexShader.Get(), nullptr, 0);
			TDevice::m_pd3dContext->PSSetShader(
				m_pShader->m_pPixelShader.Get(), nullptr, 0);
		}
		PostRender();
		return;
	}
	TObject2D::Render();
}
void TEffectObj::SetVertexData()
{
	TObject2D::SetVertexData();
	if (m_Data.m_iType == 0 && m_pTexture)
	{
		float xSize = m_pTexture->m_TexDesc.Width;
		float ySize = m_pTexture->m_TexDesc.Height;
		TRect rt;
		rt.SetS(m_rtList[0].left,
			m_rtList[0].top,
			m_rtList[0].right,
			m_rtList[0].bottom);
		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
	}
}