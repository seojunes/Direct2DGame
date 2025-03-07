#include "AActor.h"
#include "TDevice.h"
AActor::AActor() {
	CreateConstantBuffer();
};
AActor::~AActor() {};
void AActor::SetMesh(UStaticMeshComponent* mesh)
{
	Mesh = mesh;
}
bool	AActor::CreateConstantBuffer()
{
	// 화면좌표계  <-> 변환  <-> 직각좌표계
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(cbData);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_cbData;
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void AActor::Init() {
	TVector3 vCameraPos = TVector3(0, 0, -100.0f);
	TVector3 vCameraTarget = TVector3(0, 0, 0.0f);
	TVector3 vCameraUp = TVector3(0, 1.0f, 0.0f);
	m_cbData.matView = TMatrix::CreateViewMatrix(vCameraPos, vCameraTarget, vCameraUp);
	float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	float fFovY = (float)T_Pi * 0.25f;
	float fNearZ = 1.0f;
	float fFarZ = 1000.0f;
	m_cbData.matProj = TMatrix::CreateProjMatrix(fFovY, fAspect, fNearZ, fFarZ);
	m_cbData.matView.Transpose();
	m_cbData.matProj.Transpose();
}
void AActor::Tick()
{

	if (Mesh != nullptr) Mesh->Tick();
}
void AActor::Render() {
	static TVector3 vCameraPos = TVector3(0, 0, -10.0f);
	if (g_GameKey.dwAkey == KEY_HOLD)
	{
		vCameraPos.z += 10 * g_fSPF;
	}
	if (g_GameKey.dwDkey == KEY_HOLD)
	{
		vCameraPos.z -= 10 * g_fSPF;
	}
	if (g_GameKey.dwSkey == KEY_HOLD)
	{
		vCameraPos.x += 10 * g_fSPF;
	}
	if (g_GameKey.dwWkey == KEY_HOLD)
	{
		vCameraPos.x -= 10 * g_fSPF;
	}
	
	//vCameraPos.z += cosf(g_fGT) * 100.0f * g_fSPF;
	TVector3 vCameraTarget = TVector3(0, 0, 0.0f);
	TVector3 vCameraUp = TVector3(0, 1.0f, 0.0f);
	m_cbData.matView = TMatrix::CreateViewMatrix(vCameraPos, vCameraTarget, vCameraUp);
	float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	float fFovY = (float)T_Pi * 0.25f;
	float fNearZ = 1.0f;
	float fFarZ = 1000.0f;
	m_cbData.matProj = TMatrix::CreateProjMatrix(fFovY, fAspect, fNearZ, fFarZ);

	//m_cbData.matWorld.RotateZ(g_fGT);

	m_cbData.matView = m_cbData.matView.Transpose();
	m_cbData.matProj = m_cbData.matProj.Transpose();
	m_cbData.matWorld = m_cbData.matWorld.Transpose();

	TDevice::m_pd3dContext->UpdateSubresource(
		m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	// 0번 레지스터에 셰이더상수 m_pConstantBuffer를 연결
	TDevice::m_pd3dContext->VSSetConstantBuffers(
		0, 1, m_pConstantBuffer.GetAddressOf());
	if (Mesh != nullptr)
	{
		Mesh->Render();
	}
}
void AActor::Destroy() {
	if (Mesh != nullptr) Mesh->Destroy();
}