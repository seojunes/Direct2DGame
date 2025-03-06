#include "UPrimitiveComponent.h"
#include "TDevice.h"
bool	UPrimitiveComponent::CreateVertexBuffer()
{
	// 화면좌표계  <-> 변환  <-> 직각좌표계
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_vVertexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vVertexList.at(0);
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool	UPrimitiveComponent::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(DWORD) * m_vIndexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vIndexList.at(0);
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pIndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void   UPrimitiveComponent::Init()
{

}
void   UPrimitiveComponent::Tick()
{
};
void   UPrimitiveComponent::Render()
{
};
void   UPrimitiveComponent::Destroy()
{
};