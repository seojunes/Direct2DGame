#include "UPrimitiveComponent.h"
#include "TDevice.h"
bool	UPrimitiveComponent::CreateVertexBuffer()
{
	if (m_vVertexList.size() == 0) return true;
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
	if (m_vIWList.size() == 0)
	{
		m_vIWList.resize(m_vVertexList.size());
	}
	bd.ByteWidth = sizeof(IW_VERTEX) * m_vVertexList.size();
	sd.pSysMem = &m_vIWList.at(0);
	hr = TDevice::m_pd3dDevice->CreateBuffer(&bd, &sd, m_pVertexIWBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool	UPrimitiveComponent::CreateIndexBuffer()
{
	if (m_vIndexList.size() == 0) return true;
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
	for (auto child : m_SubChilds)
	{
		if (child->m_bRenderMesh == false) continue;
		child->Render();
	}
	PreRender();
	PostRender();

};
void  UPrimitiveComponent::SetMaterial(
	std::shared_ptr<UMaterial> pMaterial)
{
	m_pMaterial = pMaterial;
}
void	UPrimitiveComponent::PreRender()
{
	if (m_pMaterial)
	{
		if (m_pMaterial->m_pTexture)
		{
			TDevice::m_pd3dContext->PSSetShaderResources(0, 1, &m_pMaterial->m_pTexture->m_pTexSRV);
		}
		TDevice::m_pd3dContext->VSSetShader(m_pMaterial->m_pShader->m_pVertexShader.Get(), nullptr, 0);
		TDevice::m_pd3dContext->PSSetShader(m_pMaterial->m_pShader->m_pPixelShader.Get(), nullptr, 0);
		TDevice::m_pd3dContext->IASetInputLayout(m_pMaterial->m_pInputLayout->Get());
	}
	// 정점버퍼에서 Offsets에서 시작하여
	// Strides크기로 정점을 정점쉐이더로 전달해라.
	UINT Strides[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) };
	UINT Offsets[2] = { 0,0 };
	ID3D11Buffer* pVB[2] = { m_pVertexBuffer.Get(),
							 m_pVertexIWBuffer.Get() };
	TDevice::m_pd3dContext->IASetVertexBuffers(
		0,
		2,
		pVB,
		Strides,
		Offsets);
	TDevice::m_pd3dContext->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, 0);
	//TDevice::m_pd3dContext->IASetPrimitiveTopology(
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void	UPrimitiveComponent::PostRender()
{
	if (m_pIndexBuffer == nullptr)
		TDevice::m_pd3dContext->Draw(m_vVertexList.size(), 0);
	else
		TDevice::m_pd3dContext->DrawIndexed(m_vIndexList.size(), 0, 0);
}
void   UPrimitiveComponent::Destroy()
{
};