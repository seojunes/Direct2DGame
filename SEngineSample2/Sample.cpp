#include "Sample.h"

void Sample::Init()
{
	m_pGameCamera = std::make_shared<SBackViewCamera>();
	g_pCamera = m_pGameCamera.get();
	g_pCamera->m_fPitch = T_Pi * 0.25f;
	TVector3 vCameraPos = TVector3(0, 0, -10.0f);
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
	pMaterial->Load(L"../../data/shader/pnct.txt",	L"../../data/texture/kgca08.bmp");
	m_SMLine->GetMesh()->SetMaterial(pMaterial);

	m_SMBox = std::make_shared<AActor>();
	m_SMBox->Init();
	//auto data = Load(L"box.fbx");
	m_SMBox->SetMesh(GetShape(L"box"));
	//m_SMBox->SetMesh(Load("Sphere.fbx"));
	m_Materials.emplace_back(std::make_shared<UMaterial>());
	m_Materials.emplace_back(std::make_shared<UMaterial>());
	m_Materials.emplace_back(std::make_shared<UMaterial>());
	m_Materials[0]->Load(L"../../data/shader/pnct.txt",	L"../../data/texture/hero.png");
	m_Materials[1]->Load(L"../../data/shader/pnct.txt",	L"../../data/texture/gg.bmp");
	m_Materials[2]->Load(L"../../data/shader/pnct.txt",	L"../../data/texture/_RAINBOW.bmp");
	m_SMBox->GetMesh()->SetMaterial(m_Materials[0]);

	m_SMHero = std::make_shared<AActor>();
	m_SMHero->m_vPosition = { 0,0,3 };
	m_SMHero->Init();
	m_SMHero->SetMesh(GetShape(L"box"));
	m_Materials.emplace_back(std::make_shared< UMaterial>(L"../../data/shader/pnct.txt",L"../../data/texture/_RAINBOW.bmp"));
	m_SMHero->GetMesh()->SetMaterial(m_Materials.back());

	m_SMPlane = std::make_shared<AActor>();
	m_SMPlane->Init();
	m_SMPlane->SetMesh(GetShape(L"plane"));
	auto pMaterialPlane = std::make_shared<UMaterial>();
	pMaterialPlane->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/kgca08.bmp");
	m_SMPlane->GetMesh()->SetMaterial(pMaterialPlane);

	g_pCamera->m_fPitch = T_Pi * 0.25f;
	g_pCamera->CreateViewMatrix(
		{ 0, 10, -10.0f },
		{ 0, 0, 0.0f },
		{ 0, 1.0f, 0.0f });
	fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	g_pCamera->CreateProjMatrix((float)T_Pi * 0.25f, fAspect, 1.0f, 1000.0f);
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
		TVector3 vMovement;
		float fDistance = 0.0f;
		if (g_GameKey.dwWkey == KEY_HOLD)
		{
			fDistance += g_fSPF * 10.0f;
			vMovement = m_SMHero->m_vLook * fDistance;
		}
		if (g_GameKey.dwSkey == KEY_HOLD)
		{
			fDistance -= g_fSPF * 10.0f;
			vMovement = m_SMHero->m_vLook * fDistance;
		}
		fDistance = 0.0f;
		if (g_GameKey.dwAkey == KEY_HOLD)
		{
			fDistance -= g_fSPF * 10.0f;
			vMovement += m_SMHero->m_vRight * fDistance;
		}
		if (g_GameKey.dwDkey == KEY_HOLD)
		{
			fDistance += g_fSPF * 10.0f;
			vMovement += m_SMHero->m_vRight * fDistance;
		}
		if (g_nMouseWheelDelta != 0)
		{
			fDistance = ((g_nMouseWheelDelta / 120) > 0) ? (1.0f) : (-1.0f);
			fDistance = fDistance * g_fSPF * 300.0f;
			g_nMouseWheelDelta = 0;
		}
		

		m_SMHero->m_vPosition += vMovement;
		g_pCamera->m_vPosition += vMovement;
		//TVector3 vUp = { 0,1,0 };
		g_pCamera->CreateViewMatrix(g_pCamera->m_vPosition,	m_SMHero->m_vPosition);
	}

	m_SMBox->Tick();
}
void Sample::Render() {

	m_SMPlane->m_matOffset = TMatrix::Identity();
	m_SMPlane->m_vScale = { 100,100,100 };
	m_SMPlane->m_vRotation = { T_Pi * 0.5f, 0, 0.0f };
	m_SMPlane->m_vPosition = { 0,-5.0f,0 };
	m_SMPlane->Render();

	m_SMHero->m_matOffset = TMatrix::Identity();
	m_SMHero->m_vScale = { 1,3,1 };
	m_SMHero->m_vRotation = { 0.0f, 0, 0.0f };
	m_SMHero->Render();

	m_SMBox->m_matOffset = TMatrix::Identity();
	m_SMBox->m_vScale = { 1,1,1 };
	m_SMBox->m_vRotation = { 0.0f, 0, 0.0f };
	m_SMBox->m_vPosition = { 0,0,0 };
	m_SMBox->GetMesh()->SetMaterial(m_Materials[0]);
	m_SMBox->Render();

	m_SMBox->m_matOffset.Trans(3, 0, 0);
	m_SMBox->m_vScale = { 1,1,1 };
	m_SMBox->m_vRotation = { 0.0f, g_fGT, 0.0f };
	m_SMBox->m_vPosition = { 0,0,0 };
	m_SMBox->GetMesh()->SetMaterial(m_Materials[1]);
	m_SMBox->Render();

	// direction line
	m_SMBox->m_matOffset = TMatrix::Identity();
	m_SMBox->m_vScale = { 1000,0.05f,0.05f };
	m_SMBox->m_vRotation = { 0, 0, 0.0f };
	m_SMBox->m_vPosition = { 0,0,0 };
	m_SMBox->GetMesh()->SetMaterial(m_Materials[2]);
	m_SMBox->Render();
	m_SMBox->m_vScale = { 0.05f,1000,0.05f };
	m_SMBox->Render();
	m_SMBox->m_vScale = { 0.05f,0.05f,1000 };
	m_SMBox->Render();


	TDevice::m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
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
	m_SMBox->Destroy();
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