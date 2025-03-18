#include "Sample.h"


void Sample::Init()
{
	m_pGameCamera = std::make_shared<SBackViewCamera>();
	g_pCamera = m_pGameCamera.get();
	g_pCamera->m_fPitch = T_Pi * 0.25f;
	TVector3 vCameraPos = TVector3(0, 10, -10.0f);
	TVector3 vCameraTarget = TVector3(0, 0, 0.0f);
	TVector3 vCameraUp = TVector3(0, 1.0f, 0.0f);
	m_pGameCamera->CreateViewMatrix(vCameraPos, vCameraTarget, vCameraUp);

	float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	float fFovY = (float)T_Pi * 0.25f;
	float fNearZ = 1.0f;
	float fFarZ = 1000.0f;
	m_pGameCamera->CreateProjMatrix(fFovY, fAspect, fNearZ, fFarZ);


	m_SMLine = std::make_shared<AActor>();
	m_SMLine->Init();
	m_SMLine->SetMesh(GetShape(L"line"));
	auto pMaterial = std::make_shared<UMaterial>();
	pMaterial->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/kgca08.bmp");
	m_SMLine->GetMesh()->SetMaterial(pMaterial);


	m_Materials.emplace_back(std::make_shared<UMaterial>());
	m_Materials.emplace_back(std::make_shared<UMaterial>());
	m_Materials.emplace_back(std::make_shared<UMaterial>());
	m_Materials[0]->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/hero.png");
	m_Materials[1]->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/gg.bmp");
	m_Materials[2]->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/_RAINBOW.bmp");

	m_SMHero = std::make_shared<AActor>();
	m_SMHero->m_vPosition = { 0,1,0 };
	m_SMHero->Init();
	m_SMHero->SetMesh(GetShape(L"box"));
	m_Materials.emplace_back(
		std::make_shared< UMaterial>(
			L"../../data/shader/pnct.txt",
			L"../../data/texture/_RAINBOW.bmp"));
	m_SMHero->GetMesh()->SetMaterial(m_Materials.back());

	m_SMPlane = std::make_shared<AActor>();
	m_SMPlane->Init();
	m_SMPlane->SetMesh(GetShape(L"plane"));
	auto pMaterialPlane = std::make_shared<UMaterial>();
	pMaterialPlane->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/kgca08.bmp");
	m_SMPlane->GetMesh()->SetMaterial(pMaterialPlane);

}
void Sample::Tick()
{
	if (m_Input.KeyCheck('P') == KEY_PUSH)
	{
		SetCamera();
	}
	if (m_Input.KeyCheck('L') == KEY_PUSH)
	{
		SetCamera(m_pGameCamera.get());
	}

	if (g_pCamera == m_pGameCamera.get())
	{
		/*	float fYaw = 0;
			float fPitch = 0;
			fYaw = -g_ptDeltaMouse.x * g_fSPF;
			fPitch = -g_ptDeltaMouse.y * g_fSPF;*/
		TVector3 vMovement;
		float fDistance = 0.0f;
		bool bWSkey = false;
		if (g_GameKey.dwWkey == KEY_HOLD)
		{
			fDistance += g_fSPF * 10.0f;
			bWSkey = true;
		}
		if (g_GameKey.dwSkey == KEY_HOLD)
		{
			fDistance -= g_fSPF * 10.0f;
			bWSkey = true;
		}
		if (g_nMouseWheelDelta != 0)
		{
			fDistance = ((g_nMouseWheelDelta / 120) > 0) ? (1.0f) : (-1.0f);
			fDistance = fDistance * g_fSPF * 300.0f;
			g_nMouseWheelDelta = 0;
		}


		auto dirZ = g_pCamera->m_vLook;
		dirZ.y = 0.0f;
		dirZ.Normalize();
		auto dirX = (TVector3(0, 1, 0) ^ dirZ).Normal();
		vMovement = dirZ * fDistance;
		m_SMHero->m_vPosition += vMovement;

		if (bWSkey == true)
		{
			m_SMHero->m_matWorld._11 = dirX.x;
			m_SMHero->m_matWorld._12 = dirX.y;
			m_SMHero->m_matWorld._13 = dirX.z;
			m_SMHero->m_matWorld._21 = 0.0f;
			m_SMHero->m_matWorld._22 = 1.0f;
			m_SMHero->m_matWorld._23 = 0.0f;
			m_SMHero->m_matWorld._31 = dirZ.x;
			m_SMHero->m_matWorld._32 = dirZ.y;
			m_SMHero->m_matWorld._33 = dirZ.z;
			m_SMHero->m_matWorld._41 = m_SMHero->m_vPosition.x;
			m_SMHero->m_matWorld._42 = m_SMHero->m_vPosition.y;
			m_SMHero->m_matWorld._43 = m_SMHero->m_vPosition.z;
		}
		fDistance = 0.0f;
		TVector3 vCameraMovement;
		if (g_GameKey.dwAkey == KEY_HOLD)
		{
			fDistance -= g_fSPF * 10.0f;
			//vMovement += m_SMHero->m_vRight * fDistance;
			vCameraMovement += g_pCamera->m_vRight * fDistance;
		}
		if (g_GameKey.dwDkey == KEY_HOLD)
		{
			fDistance += g_fSPF * 10.0f;
			//vMovement += m_SMHero->m_vRight * fDistance;
			vCameraMovement += g_pCamera->m_vRight * fDistance;
		}


		g_pCamera->m_vPosition += vMovement + vCameraMovement;
		g_pCamera->CreateViewMatrix(
			g_pCamera->m_vPosition,
			m_SMHero->m_vPosition);
		g_pCamera->UpdateVector();
	}
}
void Sample::Render() {

	m_SMPlane->m_matOffset = TMatrix::Identity();
	m_SMPlane->m_vScale = { 100,100,100 };
	m_SMPlane->m_vRotation = { T_Pi * 0.5f, 0, 0.0f };
	m_SMPlane->m_vPosition = { 0,0.0f,0 };

	m_SMPlane->m_matScale.Scale(m_SMPlane->m_vScale);
	TMatrix matX, matY, matZ;
	matX.RotateX(m_SMPlane->m_vRotation.x);
	matY.RotateY(m_SMPlane->m_vRotation.y);
	matZ.RotateZ(m_SMPlane->m_vRotation.z);
	m_SMPlane->m_matRotation = matZ * matX * matY;

	m_SMPlane->m_matTrans.Trans(m_SMPlane->m_vPosition);
	m_SMPlane->m_matWorld =
		m_SMPlane->m_matOffset *
		m_SMPlane->m_matScale *
		m_SMPlane->m_matRotation *
		m_SMPlane->m_matTrans *
		m_SMPlane->m_matParent;
	m_SMPlane->Render();

	m_SMHero->m_matOffset = TMatrix::Identity();
	m_SMHero->Render();


	TDevice::m_pd3dContext->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	m_SMLine->m_vScale = { 1000,1,1 };
	m_SMLine->Render();
	m_SMLine->m_vScale = { 1,1000,1 };
	m_SMLine->Render();
	m_SMLine->m_vScale = { 1,1,1000 };
	m_SMLine->Render();
}
void Sample::Destroy()
{
	m_SMHero->Destroy();
	m_SMLine->Destroy();
	m_SMPlane->Destroy();
}

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	Sample sample;
	sample.SetWindowClass(hInstance);
	sample.SetWindow(L"test", 800, 600);
	sample.GameRun();
}