#include "UStaticMeshComponent.h"
#include "TDevice.h"
void  UStaticMeshComponent::SetMaterial(
	std::shared_ptr<UMaterial> pMaterial)
{
	m_pMaterial = pMaterial;
}
void   UStaticMeshComponent::Init()
{

}
void   UStaticMeshComponent::Tick()
{
};
void   UStaticMeshComponent::Destroy()
{
};
void	UStaticMeshComponent::PreRender()
{
	if (m_pMaterial)
	{
		TDevice::m_pd3dContext->PSSetShaderResources(
			0, 1, &m_pMaterial->m_pTexture->m_pTexSRV);
		TDevice::m_pd3dContext->VSSetShader(
			m_pMaterial->m_pShader->m_pVertexShader.Get(), nullptr, 0);
		TDevice::m_pd3dContext->PSSetShader(
			m_pMaterial->m_pShader->m_pPixelShader.Get(), nullptr, 0);
		TDevice::m_pd3dContext->IASetInputLayout(
			m_pMaterial->m_pInputLayout->Get());
	}
	// 정점버퍼에서 Offsets에서 시작하여
	// Strides크기로 정점을 정점쉐이더로 전달해라.
	UINT Strides = sizeof(PNCT_VERTEX);
	UINT Offsets = 0;
	TDevice::m_pd3dContext->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&Strides,
		&Offsets);
	TDevice::m_pd3dContext->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, 0);
	TDevice::m_pd3dContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void	UStaticMeshComponent::Render()
{
	PreRender();
	PostRender();
}
void	UStaticMeshComponent::PostRender()
{
	if (m_pIndexBuffer == nullptr)
		TDevice::m_pd3dContext->Draw(m_vVertexList.size(), 0);
	else
		TDevice::m_pd3dContext->DrawIndexed(m_vIndexList.size(), 0, 0);
}