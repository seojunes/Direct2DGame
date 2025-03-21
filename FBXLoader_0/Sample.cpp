#include "Sample.h"


void Sample::Init()
{
	std::vector<std::string> list =
	{
		{"../../data/fbx/box.fbx"},
		{"../../data/fbx/SM_Barrel.fbx"},
		{"../../data/fbx/sphereBox.fbx"},
		{"../../data/fbx/MultiCamera/MultiCameras.fbx"},
		{"../../data/fbx/ship/ship.fbx"},
	};

	m_FbxObjs.resize(list.size());
	for (int iObj = 0; iObj < list.size(); iObj++)
	{
		m_FbxObjs[iObj] = std::make_shared<AActor>();
		m_FbxObjs[iObj]->Init();

		auto fbxobj = m_FbxObjs[iObj].get();
		if (m_FbxImporter.Load(list[iObj], fbxobj))
		{
			// 몇개의 오브젝트가 있느냐?
			for (int iMesh = 0; iMesh < m_FbxObjs[iObj]->GetMesh()->m_Childs.size(); iMesh++)
			{
				auto child = m_FbxObjs[iObj]->GetMesh()->m_Childs[iMesh];
				if (child->m_SubChilds.size() == 0)
				{
					auto pMaterialPlane = std::make_shared<UMaterial>();
					std::wstring texPath = L"../../data/fbx/";
					if (child->m_csTextures.size() == 0)
					{
						texPath += L"kgca08.bmp";
					}
					else
					{
						texPath += child->m_csTextures[0];
					}

					pMaterialPlane->Load(L"../../data/shader/object.txt",
						texPath);
					child->SetMaterial(pMaterialPlane);

					child->CreateVertexBuffer();
					child->CreateIndexBuffer();
				}
				else
				{
					for (int iSubMaterial = 0;
						iSubMaterial < child->m_SubChilds.size();
						iSubMaterial++)
					{
						auto sub = child->m_SubChilds[iSubMaterial];
						sub->CreateVertexBuffer();
						sub->CreateIndexBuffer();

						auto pMaterialPlane = std::make_shared<UMaterial>();
						std::wstring texPath = L"../../data/fbx/";
						texPath += child->m_csTextures[iSubMaterial];
						pMaterialPlane->Load(L"../../data/shader/object.txt", texPath);
						sub->SetMaterial(pMaterialPlane);
					}
				}
			}
		}
	}
	//g_pCamera->m_fPitch = T_Pi * 0.25f;
	g_pCamera->CreateViewMatrix(
		{ 0, 0, -100.0f },
		{ 0, 0, 0.0f },
		{ 0, 1.0f, 0.0f });
	float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
	g_pCamera->CreateProjMatrix((float)T_Pi * 0.25f, fAspect, 1.0f, 1000.0f);
}
void Sample::Tick()
{
	int index = 0;
	for (auto obj : m_FbxObjs)
	{
		obj->m_vPosition = { -200.0f + 100.0f * index++,0,0 };
		obj->Tick();
	}
}
void Sample::Render() {

	for (auto obj : m_FbxObjs)
	{
		obj->Render();
	}
}
void Sample::Destroy()
{
	for (auto obj : m_FbxObjs)
	{
		obj->Destroy();
	}
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