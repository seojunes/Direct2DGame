#include "Sample.h"

void Sample::Init()
{
	//std::vector<std::string> list =
	//{
	//	//{"../../data/fbx/Turret_Deploy1.fbx"},
	//	//{"../../data/fbx/MultiCamera/MultiCameras.fbx"},
	//	/*{"../../data/fbx/box.fbx"},
	//	{"../../data/fbx/SM_Barrel.fbx"},
	//	{"../../data/fbx/sphereBox.fbx"},
	//	{"../../data/fbx/MultiCamera/MultiCameras.fbx"},
	//	{"../../data/fbx/ship/ship.fbx"},*/
	//};
	std::vector<std::string> list =
	{
		//{"../../data/fbx/Man.fbx"},
		//{"../../data/fbx/Man.fbx"},
		{"../../data/fbx/SKM_Manny.fbx"},
		//{"../../data/fbx/MM_Idle.fbx"},
		{"../../data/fbx/MM_Walk_Fwd.fbx"},
		//{"../../data/fbx/MM_Run_Fwd.fbx"},
		/*{"../../data/fbx/MM_run.fbx"},
		{"../../data/fbx/MM_walk.fbx"},
		{"../../data/fbx/Man.fbx"},	*/
	};
	std::vector<std::wstring> assetlist =
	{
		//{L"../../data/fbx/Man.asset"},
		//{L"../../data/fbx/Man.asset"},
		{L"../../data/fbx/SKM_Manny.asset"},
		//{L"../../data/fbx/MM_Idle.asset"},
		{L"../../data/fbx/MM_Walk_Fwd.asset"},
		//{L"../../data/fbx/MM_Run_Fwd.asset"},
		/*{L"../../data/fbx/MM_run.asset"},
		{L"../../data/fbx/MM_walk.asset"},
		{L"../../data/fbx/Man.asset"},	*/
	};
	// asset 출력
	/*for (int iObj = 0; iObj < list.size(); iObj++)
	{
		auto asset= std::make_shared<AAsset>();
		m_FbxImporter.reset();
		if (m_FbxImporter.Load(list[iObj], asset.get()))
		{
			AAsset::Export(asset.get(), assetlist[iObj]);
		}
	}*/
	//// asset 로드
	for (int iObj = 0; iObj < list.size(); iObj++)
	{
		auto asset = std::make_shared<AAsset>();
		if(AAsset::Import(assetlist[iObj], asset ))
		{			
		}
		m_FbxObjs.emplace_back(asset);
	}

	D3D11_INPUT_ELEMENT_DESC layoutiw[] =
	{
		// 0~8
		{ "POS",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",  0, DXGI_FORMAT_R32G32_FLOAT,			0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumCnt = sizeof(layoutiw) / sizeof(layoutiw[0]);

	m_FbxObjs.resize(list.size());
	for (int iObj = 0; iObj < list.size(); iObj++)
	{
		//m_FbxObjs[iObj] = std::make_shared<AAsset>();
		m_FbxObjs[iObj]->Init();

		auto fbxobj = m_FbxObjs[iObj].get();		
		//m_FbxImporter.reset();
		//if (m_FbxImporter.Load(list[iObj], fbxobj))
		{
			// 몇개의 오브젝트가 있느냐?
			for (int iMesh = 0; iMesh < m_FbxObjs[iObj]->GetMesh()->m_Childs.size(); iMesh++)
			{				

				m_FbxObjs[iObj]->m_CurrentAnimMatrix.resize(m_FbxObjs[iObj]->GetMesh()->m_Childs.size());
				auto child = m_FbxObjs[iObj]->GetMesh()->m_Childs[iMesh];
				for (int i = 0; i < m_FbxObjs[iObj]->GetMesh()->m_Childs.size(); i++)
				{
					if (_tcscmp(fbxobj->m_ptNodeList[iMesh].m_szParentName,
						fbxobj->m_ptNodeList[i].m_szName) == 0)
					{
						child->m_pParent = 
							m_FbxObjs[iObj]->GetMesh()->m_Childs[i].get();
						break;
					}
				}

				if (child->m_bRenderMesh)
				{
					if (child->m_SubChilds.size() == 0)
					{
						if (child->m_vVertexList.size())
						{
							auto pMaterial = std::make_shared<UMaterial>();
							std::wstring texPath = L"../../data/fbx/";
							if (child->m_csTextures.size() == 0)
							{
								texPath += L"T_Manny_01_D.PNG";
							}
							else
							{
								texPath += child->m_csTextures[0];
							}

							pMaterial->Load(L"../../data/shader/Character.txt", texPath);
							child->SetMaterial(pMaterial);
							child->CreateVertexBuffer();
							child->CreateIndexBuffer();
							if (pMaterial->m_pShader)
							{
								pMaterial->SetInputLayout(I_InputLayout.Load(
									pMaterial->m_pShader->m_pCode.Get(), layoutiw, iNumCnt, L"PNCT_IW"));
							}
						}

					}
					else
					{
						for (int iSubMaterial = 0; iSubMaterial < child->m_SubChilds.size(); iSubMaterial++)
						{
							auto sub = child->m_SubChilds[iSubMaterial];
							if (sub->m_vVertexList.size() == 0)
							{
								sub->m_bRenderMesh = false;
								continue;
							}
							/*  디버그 중복삭제
							sub->m_vIWList.resize(sub->m_vVertexList.size());
							for (int i = 0; i < sub->m_vVertexList.size(); i++)
							{
								sub->m_vIWList[i].i1[0] = iMesh;
								sub->m_vIWList[i].w1[0] = 1.0f;
							}*/
							sub->CreateVertexBuffer();
							sub->CreateIndexBuffer();

							auto pMaterial = std::make_shared<UMaterial>();
							std::wstring texPath = L"../../data/fbx/";
							/*if (child->m_csTextures[iSubMaterial].empty() == false)
							{
								texPath += child->m_csTextures[iSubMaterial];
							}
							else*/
							{
								if (iSubMaterial == 0) texPath += L"T_Manny_02_D.PNG";
								else texPath += L"T_Manny_01_D.PNG";
							}

							pMaterial->Load(L"../../data/shader/Character.txt", texPath);

							if (pMaterial->m_pShader)
							{
								pMaterial->SetInputLayout(I_InputLayout.Load(pMaterial->m_pShader->m_pCode.Get(), layoutiw, iNumCnt, L"PNCT_IW"));
							}
							sub->SetMaterial(pMaterial);
						}
					}
				}
			}
		}
	}

	m_FbxObjs[0]->m_pCurrentAnimation = m_FbxObjs[1].get();
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
	m_FbxObjs[0]->Tick();
	//for (auto obj : m_FbxObjs)
	//{
	//	//obj->m_vPosition = { -200.0f + 100.0f * index++,0,0 };
	//	obj->m_vPosition = { 0,0,0 };
	//	//obj->m_vRotation.y = g_fGT;
	//	obj->Tick();
	//}
}
void Sample::Render() {

	m_FbxObjs[0]->Render();
	/*for (auto obj : m_FbxObjs)
	{
		obj->Render();
	}*/
}
void Sample::Release()
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