#include "SFbxImporter.h"
bool  SFbxImporter::Load(
	std::string loadfile,
	AActor* actor)
{
	m_pManager = FbxManager::Create();					//FBX SDK�� ���������� (FBX �ý��� �ʱ�ȭ)
	m_pImporter = FbxImporter::Create(m_pManager, "");	// FBX������ �ε��ϴ� ��ü
	m_pScene = FbxScene::Create(m_pManager, "");		// �ε�� FBX������ ���(Scene) ������ �����ϴ� ��ü

	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);		//FBX�� ���� ��ǥ�谡 �ٸ��� �־, Maya Z-Up ��ǥ��� ��ȯ. << 

	if (!m_pImporter->Initialize(loadfile.c_str()))
	{
		Destroy();
		return false;
	}
	if (!m_pImporter->Import(m_pScene))
	{
		Destroy();
		return false;
	}
	// vb, ib, texture, animation
	m_pRootNode = m_pScene->GetRootNode();			// �� �ܰ迡�� ���� ������
	PreProcess(m_pRootNode);						// 
	for (int iMesh = 0; iMesh < m_FbxMeshs.size(); iMesh++)
	{
		ParseMesh(m_FbxMeshs[iMesh], actor);
	}
	Destroy();
	return true;
}
void SFbxImporter::ParseMesh(FbxMesh* fbxmesh, AActor* actor)
{
	auto mesh = std::make_shared<UStaticMeshComponent>();

	// ���̾� ( 1���� ������, �������� ���ļ� ������ ����)
	std::vector<FbxLayerElementUV*>				VertexUVSet;
	std::vector<FbxLayerElementVertexColor*>	VertexColorSet;
	std::vector<FbxLayerElementTangent*>		VertexTangentSet;
	std::vector<FbxLayerElementMaterial*>		MaterialSet;
	std::vector<FbxLayerElementNormal*>			VertexNormalSets;
	int iLayerCount = fbxmesh->GetLayerCount();
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pFbxLayer = fbxmesh->GetLayer(iLayer);
		if (pFbxLayer->GetUVs() != nullptr)
		{
			VertexUVSet.push_back(pFbxLayer->GetUVs());
		}
		if (pFbxLayer->GetVertexColors() != nullptr)
		{
			VertexColorSet.push_back(pFbxLayer->GetVertexColors());
		}
	}

	int iNumPolyCount = fbxmesh->GetPolygonCount();
	FbxVector4* pVertexPositions = fbxmesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = fbxmesh->GetPolygonSize(iPoly);
		// ������ 3 or 4 
		int iNumFace = iPolySize - 2;
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// INDEX -> MAX 0, 2, 1   : ������ ��ǥ��
			// INDEX -> DX  0, 1, 2   : �޼� ��ǥ��
			// ������ġ �ε���
			int CornerIndex[3];
			CornerIndex[0] = fbxmesh->GetPolygonVertex(iPoly, 0);
			CornerIndex[1] = fbxmesh->GetPolygonVertex(iPoly, iFace + 2);
			CornerIndex[2] = fbxmesh->GetPolygonVertex(iPoly, iFace + 1);

			//�ؽ�ó��ǥ �ε���
			int uvIndex[3];
			uvIndex[0] = fbxmesh->GetTextureUVIndex(iPoly, 0);
			uvIndex[1] = fbxmesh->GetTextureUVIndex(iPoly, iFace + 2);
			uvIndex[2] = fbxmesh->GetTextureUVIndex(iPoly, iFace + 1);

			for (int index = 0; index < 3; index++)
			{
				PNCT_VERTEX v;
				FbxVector4 p =
					pVertexPositions[CornerIndex[index]];
				// ������ǥ�� 
				// MAX X, Z, Y
				// DX  X, Y, Z
				v.p.x = p.mData[0];
				v.p.y = p.mData[2];
				v.p.z = p.mData[1];

				v.c.x = 1.0f;
				v.c.y = 1.0f;
				v.c.z = 1.0f;
				v.c.w = 1.0f;

				if (VertexUVSet.size() > 0)
				{
					FbxLayerElementUV* pUVSet = VertexUVSet[0];
					FbxVector2 uv;
					ReadTextureCoord(
						fbxmesh,
						pUVSet,
						CornerIndex[index],
						uvIndex[index],
						uv);

					// MAX  �ؽ�ó��ǥ��   U,V
					// DX       ""         U, 1.0f-V  
					v.t.x = uv.mData[0];
					v.t.y = 1.0f - uv.mData[1];
				}

				mesh->m_vVertexList.emplace_back(v);
			}
		}
	}
	actor->SetMesh(mesh);
}
// ����Լ��� ��� MESH�� ã�Ƽ� FbxMeshs�� ������.
void  SFbxImporter::PreProcess(FbxNode* pNode)
{
	if (pNode == nullptr) return;
	std::string nodename = pNode->GetName();
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh != nullptr)
	{
		m_FbxMeshs.emplace_back(pMesh);
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChild = pNode->GetChild(iNode);
		PreProcess(pChild);
	}
}
void SFbxImporter::Destroy()
{
	if (m_pScene)	m_pScene->Destroy();
	if (m_pImporter) m_pImporter->Destroy();
	if (m_pManager)  m_pManager->Destroy();
	m_pScene = nullptr;
	m_pImporter = nullptr;
	m_pManager = nullptr;
}
// FBX Mesh ��ü���� �ؽ�ó��ǥ (UV����)�� �����ϴ� �Լ�
void SFbxImporter::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
	int vertexIndex, int uvIndex, FbxVector2& uv)
{
	//UV set ��ȿ�� �˻�.
	FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
	if (pFbxLayerElementUV == nullptr) 
	{
		return;
	}

	switch (pFbxLayerElementUV->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pFbxLayerElementUV->GetReferenceMode())
		{
			// Always enters this part for the example model
		case FbxLayerElementUV::eDirect: // fbx 5.0 ������ ��
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
}