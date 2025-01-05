#include "TObject2D.h"
#include "TDevice.h"
void	TObject2D::Transform(TVector2 vCamera)
{
	TObject::Transform(vCamera);
	TransformCamera(vCamera);
	TransformNDC();
	TDevice::m_pd3dContext->UpdateSubresource(
		m_pMeshRender->m_pVertexBuffer.Get(), 0, nullptr,
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
		TVector2 ndc = CameraToNDC(
			m_vVertexList[i].v.x,
			m_vVertexList[i].v.y, g_ptClientSize);
		m_vVertexList[i].v = ndc;
	}
}
void    TObject2D::TransformCamera(TVector2 vCamera)
{
	for (UINT i = 0; i < m_vScreenList.size(); i++)
	{
		TVector2 c = m_vScreenList[i] - vCamera;
		m_vVertexList[i].v = c;
	}
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