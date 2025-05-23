#include "TEffectObj.h"
#include "TDevice.h"
void TEffectObj::SetData(TEffectData data)
{
	m_Data = data;
	m_rtList		= m_Data.m_rtList;
	m_iNumAnimFrame = m_Data.m_iNumAnimFrame;
	m_iAnimFrame	= 0;
	m_fLifeTime		= m_Data.m_fLifeTime;
	m_fOffsetTime	= m_fLifeTime / m_iNumAnimFrame;
	m_bLoop			= m_Data.m_bLoop;
	m_bDead			= false;
}

void TEffectObj::Frame()
{
	TVector2 s = ScreenToNDC(m_srtScreen.x, m_srtScreen.y, g_ptClientSize);
	TVector2 e = ScreenToNDC(m_srtScreen.x2,m_srtScreen.y2,g_ptClientSize);
	//if (m_Data.m_iType == 0)
	//{
	//	e = ScreenToNDC(m_srtScreen.x + m_rtList[m_iAnimFrame].right, m_srtScreen.y + m_rtList[m_iAnimFrame].bottom, g_ptClientSize);
	//	//이러면 texture 사이즈로 고정.
	//}
	m_vVertexList[0].v = s;
	m_vVertexList[1].v = { e.x, s.y };
	m_vVertexList[2].v = { s.x, e.y };
	m_vVertexList[3].v = e;	

	m_fCurrentTime += g_fSPF;
	m_fLifeTime -= g_fSPF;
	//if (m_bLoop == false)
	//{
		if (m_fLifeTime <= 0.0f) m_bDead = true;
	//}
	
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
			m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
			m_vVertexList[1].t = { (rt.x + rt.w) / xSize,rt.y / ySize };
			m_vVertexList[2].t = { rt.x / xSize,(rt.y + rt.h) / ySize };
			m_vVertexList[3].t = { (rt.x + rt.w) / xSize,(rt.y + rt.h) / ySize };
		}
	}
	if (m_Data.m_iType == 1)
	{
		m_pCurrentTexture = I_Tex.GetPtr(m_Data.m_szList[m_iAnimFrame]);
	}
	TDevice::m_pd3dContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
}
void TEffectObj::Render()
{
	if (m_Data.m_iType == 1)
	{
		PreRender();
		TDevice::m_pd3dContext->PSSetShaderResources(
			0, 1, &m_pCurrentTexture->m_pTexSRV);
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
		m_vVertexList[0].t = { rt.x / xSize,rt.y / ySize };
		m_vVertexList[1].t = { (rt.x + rt.w) / xSize,rt.y / ySize };
		m_vVertexList[2].t = { rt.x / xSize,(rt.y + rt.h) / ySize };
		m_vVertexList[3].t = { (rt.x + rt.w) / xSize,(rt.y + rt.h) / ySize };
	}
}