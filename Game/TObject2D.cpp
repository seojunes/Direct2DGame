#include "TObject2D.h"
#include "TDevice.h"
void	TObject2D::Transform(TVector2 vCamera)
{
	TransformCamera(vCamera);
	TransformNDC();
	TDevice::m_pd3dContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
}
TVector2 TObject2D::ScreenToNDC(float x, float y, POINT size)
{
	TVector2 ret = { x, y };
	ret.x = (x / size.x) * 2.0f - 1.0f; // 0 ~1
	ret.y = -((y / size.y) * 2.0f - 1.0f);
	return ret;
}
TVector2 TObject2D::CameraToNDC(float x, float y, POINT size)
{
	TVector2 ret = { x, y };
	ret.x = (x / (size.x * 0.5f)); // 0 ~1
	ret.y = -(y / (size.y * 0.5f));
	return ret;
}
void    TObject2D::TransformNDC()
{
	for (UINT i = 0; i < m_vVertexList.size(); i++)
	{
		TVector2 ndc = CameraToNDC(m_vVertexList[i].v.x,
			m_vVertexList[i].v.y, g_ptClientSize);
		m_vVertexList[i].v = ndc;
	}
}
void    TObject2D::TransformCamera(TVector2 vCamera)
{
	for (UINT i = 0; i < m_vScreenList.size(); i++)
	{
		TVector2 c;
		c.x = m_vScreenList[i].x - vCamera.x;
		c.y = m_vScreenList[i].y - vCamera.y;
		m_vVertexList[i].v = c;
	}
}
void    TObject2D::SetVertexData()
{
	// Á¤±ÔÈ­ÀåÄ¡(NDC)ÁÂÇ¥°è(x,y)	
	// v0:-1,1     0,1          v1:1, 1
	//    -1,0     0,0          1, 0
	// v2:1,-1     0,-1         v3:1,-1
	// È­¸éÁÂÇ¥°è  <-> º¯È¯  <-> Á÷°¢ÁÂÇ¥°è
	// Á÷°¢ÁÂÇ¥°è  <-> º¯È¯  <-> NDCÁÂÇ¥°è
	// NDCÁÂÇ¥°è  <-> º¯È¯  <-> Á÷°¢ÁÂÇ¥°è
	m_vScreenList.resize(4);
	m_vScreenList[0] = { m_srtScreen.x, m_srtScreen.y };
	m_vScreenList[1] = { m_srtScreen.x + m_srtScreen.w, m_srtScreen.y };
	m_vScreenList[2] = { m_srtScreen.x, m_srtScreen.y + m_srtScreen.h };
	m_vScreenList[3] = { m_srtScreen.x + m_srtScreen.w, m_srtScreen.y + m_srtScreen.h };

	m_vVertexList.resize(4);
	TVector2 s = m_vScreenList[0];
	TVector2 t = m_vScreenList[2];
	m_vVertexList[0].v = s;
	m_vVertexList[1].v = { t.x, s.y };
	m_vVertexList[2].v = { s.x, t.y };
	m_vVertexList[3].v = t;

	m_vVertexList[0].c = { 1.0f,0.0f,0.0f,1.0f };
	m_vVertexList[1].c = { 0.0f,1.0f,0.0f,1.0f };
	m_vVertexList[2].c = { 0.0f,0.0f,1.0f,1.0f };
	m_vVertexList[3].c = { 1.0f,1.0f,1.0f,1.0f };

	m_vVertexList[0].t = { 0.0f,0.0f };
	m_vVertexList[1].t = { 1.0f,0.0f };
	m_vVertexList[2].t = { 0.0f,1.0f };
	m_vVertexList[3].t = { 1.0f,1.0f };
}
void    TObject2D::SetIndexData()
{
	m_vIndexList.resize(6);
	m_vIndexList[0] = 0;
	m_vIndexList[1] = 1;
	m_vIndexList[2] = 2;
	m_vIndexList[3] = 2;
	m_vIndexList[4] = 1;
	m_vIndexList[5] = 3;
}
void TObject2D::SetPos(TVector2 p)
{
	m_vPos = p;
	UpdatePosition();
}
void TObject2D::UpdatePosition()
{
	m_srtScreen.SetS(m_vPos, { m_srtScreen.w, m_srtScreen.h });
	m_Sphere.vCenter = m_srtScreen.tCenter;
	m_Sphere.fRadius = m_srtScreen.fRadius;

	m_vScreenList[0] = { m_srtScreen.x, m_srtScreen.y };
	m_vScreenList[1] = { m_srtScreen.x2, m_srtScreen.y };
	m_vScreenList[2] = { m_srtScreen.x, m_srtScreen.y2 };
	m_vScreenList[3] = { m_srtScreen.x2, m_srtScreen.y2 };;
}
TObject2D::TObject2D()
{
	m_fSpeed = 100.0f;
}
TObject2D::TObject2D(TVector2 p)
{
	m_vPos = p;
	m_fSpeed = 100.0f;
}
TObject2D::TObject2D(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_fSpeed = 100.0f;
}