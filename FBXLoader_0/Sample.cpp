#include "Sample.h"


void Sample::Init()
{
	m_FbxBox.Init();

	//auto fbxfile = std::make_shared<TFbxFile>();
	if (m_FbxImporter.Load(
		//"../../data/fbx/box.fbx",
		//"../../data/fbx/SM_Barrel.fbx",
		//"../../data/fbx/MultiCamera/MultiCameras.fbx",
		////"../../data/fbx/ship/ship.fbx",
		"../../data/fbx/sphereBox.fbx",
		&m_FbxBox))
	{
		for (int iMesh = 0; iMesh < m_FbxBox.GetMesh()->m_Childs.size(); iMesh++)
		{
			auto child = m_FbxBox.GetMesh()->m_Childs[iMesh];
			child->CreateVertexBuffer();
			child->CreateIndexBuffer();
			auto pMaterialPlane = std::make_shared<UMaterial>();
			pMaterialPlane->Load(L"../../data/shader/object.txt",
				//L"../../data/fbx/20200428_185613.jpg");
				L"../../data/fbx/T_Pack_01_D.dds");
			child->SetMaterial(pMaterialPlane);
		}

		/*m_FbxBox.GetMesh()->CreateVertexBuffer();
		m_FbxBox.GetMesh()->CreateIndexBuffer();
		auto pMaterialPlane = std::make_shared<UMaterial>();
		pMaterialPlane->Load(L"../../data/shader/object.txt",
			//L"../../data/fbx/20200428_185613.jpg");
			L"../../data/fbx/T_Pack_01_D.dds");
		m_FbxBox.GetMesh()->SetMaterial(pMaterialPlane);*/

	}
	g_pCamera->m_fPitch = T_Pi * 0.25f;
	g_pCamera->CreateViewMatrix(
		{ 0, 100, -100.0f },
		{ 0, 0, 0.0f },
		{ 0, 1.0f, 0.0f });
	float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	g_pCamera->CreateProjMatrix((float)T_Pi * 0.25f, fAspect, 1.0f, 10000.0f);
}
void Sample::Tick()
{
	m_FbxBox.Tick();
}
void Sample::Render() {

	m_FbxBox.Render();
}
void Sample::Destroy()
{
	m_FbxBox.Destroy();
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