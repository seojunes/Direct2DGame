#include "Object2D.h"
#include "Device.h"
void	Object2D::Transform(TVector2 vCamera)
{
	Object::Transform(vCamera);
	TransformCamera(vCamera);
	TransformNDC();
	Device::m_pd3dContext->UpdateSubresource(
		m_pMeshRender->m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
}
TVector2 Object2D::ScreenToNDC(float x, float y, POINT size)
{
	TVector2 ret = { x, y };
	ret.x = (x / size.x) * 2.0f - 1.0f; // 0 ~1
	ret.y = -((y / size.y) * 2.0f - 1.0f);
	return ret;
}
TVector2 Object2D::CameraToNDC(float x, float y, POINT size)
{
	TVector2 ret = { x, y };
	ret.x = (x / (size.x * 0.5f)); // 0 ~1
	ret.y = -(y / (size.y * 0.5f));
	return ret;
}
void    Object2D::TransformNDC()
{
	for (UINT i = 0; i < m_vVertexList.size(); i++)
	{
		TVector2 ndc = CameraToNDC(
			m_vVertexList[i].v.x,
			m_vVertexList[i].v.y, g_ptClientSize);
		m_vVertexList[i].v = ndc /m_fZoom;
	}
}
void    Object2D::TransformCamera(TVector2 vCamera)
{
	for (UINT i = 0; i < m_vScreenList.size(); i++)
	{
		TVector2 c = (m_vScreenList[i] - vCamera)/m_fZoom;
		m_vVertexList[i].v = c;
	}
}

Object2D::Object2D()
{
	m_fSpeed = 100.0f;
}
Object2D::Object2D(TVector2 p)
{
	m_vPos = p;
	m_fSpeed = 100.0f;
}
Object2D::Object2D(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_fSpeed = 100.0f;
}

void Object2D::SetZoom(float zoom)
{
	if (zoom >= 2.0f)
	{
		zoom = 2.0f;
	}
	else if (zoom <= 0.5f)
	{
		zoom = 0.5f;
	}
	m_fZoom = zoom;
	// 최소 0.5배, 최대 2배
}