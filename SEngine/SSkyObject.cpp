#include "SSkyObject.h"
#include "SEngine.h"
#include "TDxState.h"
bool SSkyObject::Load(shared_ptr <UStaticMeshComponent> sm)
{
	Init();
	SetMesh(sm);

	const TCHAR* g_szSkyTextures[] =
	{
		L"../../data/sky/st00_cm_front.bmp",
		L"../../data/sky/st00_cm_back.bmp",
		L"../../data/sky/st00_cm_right.bmp",
		L"../../data/sky/st00_cm_left.bmp",
		L"../../data/sky/st00_cm_up.bmp",
		L"../../data/sky/st00_cm_down.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);

	for (int iPlane = 0; iPlane < iNumTexture; iPlane++)
	{
		auto material = std::make_shared<UMaterial>();
		material->Load(L"../../data/shader/pnct.txt", g_szSkyTextures[iPlane]);
		m_Materials.emplace_back(material);
	}
	return true;
}
void SSkyObject::Render()
{
	PreRender();
	if (Mesh != nullptr)
	{
		Mesh->PreRender();
	}
	PostRender();
}
void	SSkyObject::PostRender()
{
	auto matView = SEngine::g_pCamera->m_matView;
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	m_cbData.matView = Matrix::Transpose(matView);

	TDevice::m_pd3dContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	// 0번 레지스터에 셰이더상수 m_pConstantBuffer를 연결
	TDevice::m_pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	TDevice::m_pd3dContext->RSSetState(TDxState::m_pRSSolidNone.Get());
	TDevice::m_pd3dContext->PSSetSamplers(0, 1, TDxState::m_pPointSS.GetAddressOf());
	TDevice::m_pd3dContext->OMSetDepthStencilState(TDxState::m_pDSSDepthEnableZero.Get(), 0);
	for (int iPlane = 0; iPlane < m_Materials.size(); iPlane++)
	{
		TDevice::m_pd3dContext->PSSetShaderResources(0, 1, &m_Materials[iPlane]->m_pTexture->m_pTexSRV);
		if (GetMesh()->m_pIndexBuffer == nullptr)
			TDevice::m_pd3dContext->Draw(GetMesh()->m_vVertexList.size(), 0);
		else
			TDevice::m_pd3dContext->DrawIndexed(6, 6 * iPlane, 0);
	}
}