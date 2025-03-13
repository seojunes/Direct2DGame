#include "Sample.h"
UStaticMeshComponent* Sample::Load(std::wstring filename)
{
	UStaticMeshComponent* mesh = new UStaticMeshComponent();
	TVector3 vMin = TVector3(-1, -1, -1);
	TVector3 vMax = TVector3(+1, +1, +1);
	mesh->m_vVertexList.resize(24);
	mesh->m_vIndexList.resize(6 * 2 * 3);

	// ¾Õ¸é
	mesh->m_vVertexList[0] = PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMin.z), TVector3(0.0f, 0.0f, -1.0f) ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 1.0f));
	mesh->m_vVertexList[1] = PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMin.z), TVector3(0.0f, 0.0f, -1.0f) ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 0.0f));
	mesh->m_vVertexList[2] = PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMin.z), TVector3(0.0f, 0.0f, -1.0f) ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 0.0f));
	mesh->m_vVertexList[3] = PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMin.z), TVector3(0.0f, 0.0f, -1.0f) ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 1.0f));
	// µÞ¸é																							 
	mesh->m_vVertexList[4] = PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMax.z), TVector3(0.0f, 0.0f, 1.0f)  ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 1.0f));
	mesh->m_vVertexList[5] = PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMax.z), TVector3(0.0f, 0.0f, 1.0f)  ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 1.0f));
	mesh->m_vVertexList[6] = PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMax.z), TVector3(0.0f, 0.0f, 1.0f)  ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 0.0f));
	mesh->m_vVertexList[7] = PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMax.z), TVector3(0.0f, 0.0f, 1.0f)  ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 0.0f));
	// À­¸é																						
	mesh->m_vVertexList[8] = PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMin.z), TVector3(0.0f, 1.0f, 0.0f)  ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 1.0f));
	mesh->m_vVertexList[9] = PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMax.z), TVector3(0.0f, 1.0f, 0.0f)  ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 0.0f));
	mesh->m_vVertexList[10] = PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMax.z), TVector3(0.0f, 1.0f, 0.0f) ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 0.0f));
	mesh->m_vVertexList[11] = PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMin.z), TVector3(0.0f, 1.0f, 0.0f) ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 1.0f));
	// ¾Æ·§¸é																						
	mesh->m_vVertexList[12] = PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMin.z), TVector3(0.0f, -1.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(1.0f, 1.0f));
	mesh->m_vVertexList[13] = PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMin.z), TVector3(0.0f, -1.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(0.0f, 1.0f));
	mesh->m_vVertexList[14] = PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMax.z), TVector3(0.0f, -1.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(0.0f, 0.0f));
	mesh->m_vVertexList[15] = PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMax.z), TVector3(0.0f, -1.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(1.0f, 0.0f));
	// ¿ÞÂÊ¸é																						
	mesh->m_vVertexList[16] = PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMax.z), TVector3(-1.0f, 0.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(0.0f, 1.0f));
	mesh->m_vVertexList[17] = PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMax.z), TVector3(-1.0f, 0.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(0.0f, 0.0f));
	mesh->m_vVertexList[18] = PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMin.z), TVector3(-1.0f, 0.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(1.0f, 0.0f));
	mesh->m_vVertexList[19] = PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMin.z), TVector3(-1.0f, 0.0f, 0.0f),TVector4(1, 0, 0 , 1), TVector2(1.0f, 1.0f));
	// ¿À¸¥ÂÊ¸é																						
	mesh->m_vVertexList[20] = PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMin.z), TVector3(1.0f, 0.0f, 0.0f) ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 1.0f));
	mesh->m_vVertexList[21] = PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMin.z), TVector3(1.0f, 0.0f, 0.0f) ,TVector4(1, 0, 0 , 1), TVector2(0.0f, 0.0f));
	mesh->m_vVertexList[22] = PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMax.z), TVector3(1.0f, 0.0f, 0.0f) ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 0.0f));
	mesh->m_vVertexList[23] = PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMax.z), TVector3(1.0f, 0.0f, 0.0f) ,TVector4(1, 0, 0 , 1), TVector2(1.0f, 1.0f));


	UINT iIndex = 0;
	auto& I = mesh->m_vIndexList;
	// Front
	I[iIndex++] = 0; I[iIndex++] = 1; I[iIndex++] = 2;
	I[iIndex++] = 0; I[iIndex++] = 2; I[iIndex++] = 3;
	// Back (Fix)
	I[iIndex++] = 4; I[iIndex++] = 5; I[iIndex++] = 6;
	I[iIndex++] = 4; I[iIndex++] = 6; I[iIndex++] = 7;
	// Top (Fix)
	I[iIndex++] = 8; I[iIndex++] = 9; I[iIndex++] = 10;
	I[iIndex++] = 8; I[iIndex++] = 10; I[iIndex++] = 11;
	// Bottom (Fix)
	I[iIndex++] = 12; I[iIndex++] = 13; I[iIndex++] = 14;
	I[iIndex++] = 12; I[iIndex++] = 14; I[iIndex++] = 15;
	// Left (Fix)
	I[iIndex++] = 16; I[iIndex++] = 17; I[iIndex++] = 18;
	I[iIndex++] = 16; I[iIndex++] = 18; I[iIndex++] = 19;
	// Right (Fix)
	I[iIndex++] = 20; I[iIndex++] = 21; I[iIndex++] = 22;
	I[iIndex++] = 20; I[iIndex++] = 22; I[iIndex++] = 23;

	mesh->CreateVertexBuffer();
	mesh->CreateIndexBuffer();

	return mesh;
}
void Sample::Init()
{
	m_pActor = std::make_shared<AActor>();
	m_pActor->Init();
	auto data = Load(L"box.fbx");
	m_pActor->SetMesh(data);
	//m_pActor->SetMesh(Load("Sphere.fbx"));
	auto pMaterial = std::make_shared<UMaterial>();
	pMaterial->Load(L"../../data/shader/pnct.txt",
		L"../../data/texture/restart.png");
	m_pActor->GetMesh()->SetMaterial(pMaterial);
}
void Sample::Tick() {
	m_pActor->Tick();
}
void Sample::Render() {
	m_pActor->Render();
}
void Sample::Destroy() {
	m_pActor->Destroy();
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