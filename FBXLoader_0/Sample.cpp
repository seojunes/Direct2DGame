#include "Sample.h"


void Sample::Init()
{
	m_FbxBox.Init();

	//auto fbxfile = std::make_shared<TFbxFile>();
	m_FbxImporter.Load(	//"../../data/fbx/box.fbx",
						"../../data/fbx/SM_Barrel.fbx",
						&m_FbxBox);

	/*for (int iMesh = 0; iMesh < fbxfile->m_lists.size(); iMesh++)
	{
		m_FbxBox.GetMesh()->CreateVertexBuffer();
		m_FbxBox.GetMesh()->CreateIndexBuffer();
	}*/

	m_FbxBox.GetMesh()->CreateVertexBuffer();
	m_FbxBox.GetMesh()->CreateIndexBuffer();

	auto pMaterialPlane = std::make_shared<UMaterial>();
	pMaterialPlane->Load(L"../../data/shader/pnct.txt",
		//L"../../data/fbx/20200428_185613.jpg");
		L"../../data/fbx/T_Pack_01_D.dds");
	m_FbxBox.GetMesh()->SetMaterial(pMaterialPlane);

	g_pCamera->m_fPitch = T_Pi * 0.25f;
	g_pCamera->CreateViewMatrix(
		{ 0, 100, -100.0f },
		{ 0, 0, 0.0f },
		{ 0, 1.0f, 0.0f });
	float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	g_pCamera->CreateProjMatrix((float)T_Pi * 0.25f, fAspect, 1.0f, 1000.0f);
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