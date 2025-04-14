#include "AActor.h"
#include "TDevice.h"
#include "SEngine.h"

AActor::AActor() : m_vScale(1, 1, 1)
{
	//CreateConstantBuffer();
};

AActor::~AActor() {};

void AActor::SetMesh(std::shared_ptr<UStaticMeshComponent> mesh)
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
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(&bd, &sd, m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	/// 에니메이션 행렬(현재 프레임)
	bd.ByteWidth = sizeof(cbAnimData);
	sd.pSysMem = &m_cbAnimData;
	hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pCurrentAnimationCB.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void AActor::Init() 
{
	CreateConstantBuffer();
}
void AActor::Tick()
{
	//UpdateVector();
	if (Mesh != nullptr) Mesh->Tick();
	if (Mesh != nullptr)
	{
		/*m_fFrame += g_fSPF * 30 * 1.0f;
		if (m_fFrame >= 50) m_fFrame = 0;*/
		if (m_bInc)
		{
			m_fFrame += g_fSPF * 30 * 1.0f;
		}
		else
		{
			m_fFrame -= g_fSPF * 30 * 1.0f;
		}

		if (m_fFrame >= 50)
		{
			m_bInc = false;
		}
		else if (m_fFrame <= 0)
		{
			m_bInc = true;
		}
		for (int iChild = 0; iChild < Mesh->m_Childs.size(); iChild++)
		{
			m_CurrentAnimMatrix[iChild] = Mesh->m_Childs[iChild]->m_AnimList[(int)m_fFrame];
			m_cbAnimData.matBone[iChild] = TMatrix::Transpose(m_CurrentAnimMatrix[iChild]);
		}
		TDevice::m_pd3dContext->UpdateSubresource(
			m_pCurrentAnimationCB.Get(), 0, NULL, &m_cbAnimData, 0, 0);

	}
}
void AActor::PreRender()
{
	UpdateVector();

	m_cbData.matView = TMatrix::Transpose(SEngine::g_pCamera->m_matView);
	m_cbData.matProj = TMatrix::Transpose(SEngine::g_pCamera->m_matProj);
	m_cbData.matWorld = TMatrix::Transpose(m_matWorld);
}
void AActor::Render()
{
	PreRender();
	PostRender();
}
void AActor::PostRender()
{
	// 0번 레지스터에 셰이더상수 m_pConstantBuffer를 연결
	TDevice::m_pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	TDevice::m_pd3dContext->VSSetConstantBuffers(2, 1, m_pCurrentAnimationCB.GetAddressOf());
	if (Mesh != nullptr)
	{
		//for (auto child : Mesh->m_Childs)
		for (int iChild = 0; iChild < Mesh->m_Childs.size(); iChild++)
		{
			m_cbData.vData.z = iChild;
			m_cbData.vData.w = 1.0f;

			auto mesh = Mesh->m_Childs[iChild];
			if (mesh->m_bRenderMesh)
			{
				//TMatrix world = m_CurrentAnimMatrix[iChild] * m_matWorld;
				m_cbData.matWorld = TMatrix::Transpose(m_matWorld);
				TDevice::m_pd3dContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
				mesh->Render();
			}
		}
		//Mesh->Render();
	}

}

void AActor::UpdateVector()
{
	m_matScale.Scale(m_vScale);
	TMatrix matX, matY, matZ;
	matX.RotateX(m_vRotation.x);
	matY.RotateY(m_vRotation.y);
	matZ.RotateZ(m_vRotation.z);
	m_matRotation = matZ * matX * matY;

	m_matTrans.Trans(m_vPosition);
	m_matWorld =
		m_matOffset *
		m_matScale *
		m_matRotation *
		m_matTrans *
		m_matParent;

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;
	m_vRight.x = m_matWorld._11;
	m_vRight.y = m_matWorld._12;
	m_vRight.z = m_matWorld._13;
	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	m_vLook.Normalize();
	m_vRight.Normalize();
	m_vUp.Normalize();
}
void AActor::Destroy() {
	if (Mesh != nullptr) Mesh->Destroy();
}