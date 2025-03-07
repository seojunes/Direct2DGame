#include "Sample.h"
UStaticMeshComponent* Sample::Load(std::wstring filename)
{
	UStaticMeshComponent* mesh = new UStaticMeshComponent();
	TVector3 vMin = TVector3(-1, -1, -1);
	TVector3 vMax = TVector3(+1, +1, +1);
	mesh->m_vVertexList.resize(8);
	mesh->m_vIndexList.resize(6 * 2 * 3);

	// Back
	// v5  v6
	// v4  v7
	// Front   
	// v1  v2
	// v0  v3
	mesh->m_vVertexList[0] =
		PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMin.z),
			TVector3(0, 0, -1),
			TVector4(1, 0, 0, 1),
			TVector2(0, 1));
	mesh->m_vVertexList[1] =
		PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMin.z),
			TVector3(0, 0, -1),
			TVector4(1, 0, 0, 1),
			TVector2(0, 0));
	mesh->m_vVertexList[2] =
		PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMin.z),
			TVector3(0, 0, -1),
			TVector4(1, 0, 0, 1),
			TVector2(1, 0));
	mesh->m_vVertexList[3] =
		PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMin.z),
			TVector3(0, 0, -1),
			TVector4(1, 0, 0, 1),
			TVector2(1, 1));
	mesh->m_vVertexList[4] =
		PNCT_VERTEX(TVector3(vMin.x, vMin.y, vMax.z),
			TVector3(0, 0, 1),
			TVector4(1, 0, 0, 1),
			TVector2(0, 0));
	mesh->m_vVertexList[5] =
		PNCT_VERTEX(TVector3(vMin.x, vMax.y, vMax.z),
			TVector3(0, 0, 1),
			TVector4(1, 0, 0, 1),
			TVector2(0, 0));
	mesh->m_vVertexList[6] =
		PNCT_VERTEX(TVector3(vMax.x, vMax.y, vMax.z),
			TVector3(0, 0, 1),
			TVector4(1, 0, 0, 1),
			TVector2(0, 0));
	mesh->m_vVertexList[7] =
		PNCT_VERTEX(TVector3(vMax.x, vMin.y, vMax.z),
			TVector3(0, 0, 1),
			TVector4(1, 0, 0, 1),
			TVector2(0, 0));

	UINT iIndex = 0;
	auto& I = mesh->m_vIndexList;
	// Front
	I[iIndex++] = 0; I[iIndex++] = 1; I[iIndex++] = 2;
	I[iIndex++] = 0; I[iIndex++] = 2; I[iIndex++] = 3;
	// Back
	I[iIndex++] = 4; I[iIndex++] = 6; I[iIndex++] = 5;
	I[iIndex++] = 4; I[iIndex++] = 7; I[iIndex++] = 6;
	// Left
	I[iIndex++] = 4; I[iIndex++] = 5; I[iIndex++] = 1;
	I[iIndex++] = 4; I[iIndex++] = 1; I[iIndex++] = 0;
	// Right
	I[iIndex++] = 3; I[iIndex++] = 2; I[iIndex++] = 6;
	I[iIndex++] = 3; I[iIndex++] = 6; I[iIndex++] = 7;
	// Top
	I[iIndex++] = 1; I[iIndex++] = 5; I[iIndex++] = 6;
	I[iIndex++] = 1; I[iIndex++] = 6; I[iIndex++] = 2;
	// Bottom
	I[iIndex++] = 0; I[iIndex++] = 7; I[iIndex++] = 4;
	I[iIndex++] = 0; I[iIndex++] = 3; I[iIndex++] = 7;

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
		L"../../data/texture/gg.bmp");
	m_pActor->Mesh->SetMaterial(pMaterial);
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