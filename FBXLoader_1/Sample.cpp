#include "Sample.h"
/// <summary>
/// FBX 경로 리스트 생성
//  AActor 오브젝트들 생성 및 초기화
//  FBX 로더로 각 FBX를 파싱하여 메쉬 트리 구조 구축
//  각 메쉬(또는 서브메쉬)에 대해 :
//  버텍스 / 인덱스 버퍼 생성
//  셰이더 + 텍스처 + 머티리얼 바인딩
//  카메라 시점 / 투영 설정
/// </summary>

void Sample::Init()
{
	std::vector<std::string> list =
	{
		{"../../data/fbx/SKM_Manny.fbx"},
		//{"../../data/fbx/MultiCamera/MultiCameras.fbx"},
		{"../../data/fbx/Turret_Deploy1.fbx"},
		/*{"../../data/fbx/box.fbx"},
		{"../../data/fbx/SM_Barrel.fbx"},
		{"../../data/fbx/sphereBox.fbx"},
		{"../../data/fbx/MultiCamera/MultiCameras.fbx"},
		{"../../data/fbx/ship/ship.fbx"},*/
	};

	D3D11_INPUT_ELEMENT_DESC layoutiw[] =
	{
		// 0~8
		{ "POS",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",  0, DXGI_FORMAT_R32G32_FLOAT,			0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumCnt = sizeof(layoutiw) / sizeof(layoutiw[0]);

	m_FbxObjs.resize(list.size());
	for (int iObj = 0; iObj < list.size(); iObj++)
	{
		//AActor는 FX 오브젝트 하나를 표현하는 클래스(게임오브젝트 같은 역할) 초기화 작업.
		m_FbxObjs[iObj] = std::make_shared<AActor>();
		m_FbxObjs[iObj]->Init();

		//m_FbxImporter.Load 여기를 통해서 FBX파일을 읽고 파싱하여 AActor객체에 넣어준다.
		auto fbxobj = m_FbxObjs[iObj].get();
		if (m_FbxImporter.Load(list[iObj], fbxobj))
		{
			// 몇개의 오브젝트가 있느냐? 하나의 FBX파일에 여러개의 MESH가 있을수 있으므로 설정.
			for (int iMesh = 0; iMesh < m_FbxObjs[iObj]->GetMesh()->m_Childs.size(); iMesh++)
			{
				m_FbxObjs[iObj]->m_CurrentAnimMatrix.resize(m_FbxObjs[iObj]->GetMesh()->m_Childs.size());
				auto child = m_FbxObjs[iObj]->GetMesh()->m_Childs[iMesh];
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
								texPath += L"kgca08.bmp";
							}
							else
							{
								texPath += child->m_csTextures[0]; // FBX에 정의된 텍스쳐.
							}

							pMaterial->Load(L"../../data/shader/Character.txt", texPath);
							child->SetMaterial(pMaterial);
							child->CreateVertexBuffer();
							child->CreateIndexBuffer();

							if (pMaterial->m_pShader)
							{
								pMaterial->SetInputLayout(I_InputLayout.Load(pMaterial->m_pShader->m_pCode.Get(), layoutiw, iNumCnt, L"PNCT_IW"));
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

							sub->m_vIWList.resize(sub->m_vVertexList.size());
						    /*for (int i = 0; i < sub->m_vVertexList.size(); i++)
							{
								sub->m_vIWList[i].i[0] = iMesh;
								sub->m_vIWList[i].w[0] = 1.0f;
							}*/
							sub->CreateVertexBuffer();
							sub->CreateIndexBuffer();

							auto pMaterial = std::make_shared<UMaterial>();
							std::wstring texPath = L"../../data/fbx/";
							//if (child->m_csTextures[iSubMaterial].empty() == false)
							{
								texPath += child->m_csTextures[iSubMaterial];
								pMaterial->Load(L"../../data/shader/Character.txt", texPath);

							}
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
		//obj->m_vPosition = { -200.0f + 100.0f * index++,0,0 };
		obj->m_vPosition = { 10 , 10 , 10 };
		//obj->m_vRotation.y = g_fGT;
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

