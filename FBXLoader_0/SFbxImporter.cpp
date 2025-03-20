#include "SFbxImporter.h"
bool  SFbxImporter::Load(std::string loadfile, AActor* actor)
{
	m_pManager = FbxManager::Create();					//FBX SDK�� ���������� (FBX �ý��� �ʱ�ȭ)
	FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);		// ���� ����� ���� 
	m_pManager->SetIOSettings(ios);
	m_pImporter = FbxImporter::Create(m_pManager, "");	// FBX������ �ε��ϴ� ��ü
	m_pScene = FbxScene::Create(m_pManager, "");		// �ε�� FBX������ ���(Scene) ������ �����ϴ� ��ü

	//FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);		//FBX�� ���� ��ǥ�谡 �ٸ��� �־, Maya Z-Up ��ǥ��� ��ȯ. << 

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

	FbxAxisSystem	 m_SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);
	FbxSystemUnit	m_SceneSystemUnit = m_pScene->GetGlobalSettings().GetSystemUnit();

	if (m_SceneSystemUnit.GetScaleFactor() != 1)	//if (m_SceneSystemUnit != FbxSystemUnit::cm)
	{
		FbxSystemUnit::cm.ConvertScene(m_pScene);
	}

	// vb, ib, texture, animation
	m_pRootNode = m_pScene->GetRootNode();			// �� �ܰ迡�� ���� ������
	PreProcess(m_pRootNode);						// 

	auto mesh = std::make_shared<UStaticMeshComponent>();
	for (int iMesh = 0; iMesh < m_FbxMeshs.size(); iMesh++)
	{
		auto child = std::make_shared<UPrimitiveComponent>();
		ParseMesh(m_FbxMeshs[iMesh], child.get());
		mesh->m_Childs.emplace_back(child);
	}
	actor->SetMesh(mesh);
	Destroy();
	return true;
}
void SFbxImporter::ParseMesh(FbxMesh* fbxmesh, UPrimitiveComponent* actor)
{
	/// �������(�ʱ� ���� ��ġ�� ��ȯ �� �� ���)
	FbxNode* pNode = fbxmesh->GetNode();
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix matGlobal = pNode->EvaluateGlobalTransform(0);
	if (matGlobal.IsIdentity() == false)
	{
		geom = matGlobal * geom;
	}

	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	// ���̾� ( 1���� ������, �������� ���ļ� ������ ����)
	std::vector<FbxLayerElementUV*>				VertexUVSet;
	std::vector<FbxLayerElementVertexColor*>	VertexColorSet;
	std::vector<FbxLayerElementTangent*>		VertexTangentSet;
	std::vector<FbxLayerElementMaterial*>		MaterialSet;
	std::vector<FbxLayerElementNormal*>			VertexNormalSets;
	int iLayerCount = fbxmesh->GetLayerCount();
	// �츮�� iLayerCount�� 1�� ���� �۾��ϸ� �ȴ�.
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
		if (pFbxLayer->GetNormals() != NULL)
		{
			VertexNormalSets.push_back(pFbxLayer->GetNormals());
		}
	}

	int iNumPolyCount = fbxmesh->GetPolygonCount();
	FbxVector4* pVertexPositions = fbxmesh->GetControlPoints();
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = fbxmesh->GetPolygonSize(iPoly);
		// ������ 3(�ﰢ�� ���) or 4(�簢�� ���) 
		int iNumFace = iPolySize - 2;
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			int VertexIndex[3] = { 0, iFace + 2, iFace + 1 };
			// INDEX -> MAX 0, 2, 1   : ������ ��ǥ��
			// INDEX -> DX  0, 1, 2   : �޼� ��ǥ��
			// ������ġ �ε���
			int CornerIndex[3];
			CornerIndex[0] = fbxmesh->GetPolygonVertex(iPoly, VertexIndex[0]);
			CornerIndex[1] = fbxmesh->GetPolygonVertex(iPoly, VertexIndex[1]);
			CornerIndex[2] = fbxmesh->GetPolygonVertex(iPoly, VertexIndex[2]);

			//�ؽ�ó��ǥ �ε���
			int uvIndex[3];
			uvIndex[0] = fbxmesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
			uvIndex[1] = fbxmesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
			uvIndex[2] = fbxmesh->GetTextureUVIndex(iPoly, VertexIndex[2]);

			for (int index = 0; index < 3; index++)
			{
				PNCT_VERTEX v;
				FbxVector4 p = geom.MultT(pVertexPositions[CornerIndex[index]]);
				//FbxVector4 p = pVertexPositions[CornerIndex[index]];
				// ������ǥ�� 
				// MAX X, Z, Y
				// DX  X, Y, Z
				v.p.x = p.mData[0];
				v.p.y = p.mData[2];
				v.p.z = p.mData[1];

				FbxColor color = FbxColor(1, 1, 1, 1);
				if (VertexColorSet.size() > 0)
				{
					color = ReadColor(
						fbxmesh,
						VertexColorSet.size(),
						VertexColorSet[0],
						CornerIndex[index],
						iBasePolyIndex + VertexIndex[index]);
				}
				v.c.x = color.mRed;
				v.c.y = color.mGreen;
				v.c.z = color.mBlue;
				v.c.w = 1.0f;

				if (VertexNormalSets.size() > 0)
				{
					FbxVector4 finalNorm = ReadNormal(fbxmesh, VertexNormalSets.size(),
						VertexNormalSets[0],
						CornerIndex[index],
						iBasePolyIndex + VertexIndex[index]);
					finalNorm.mData[3] = 0.0;
					finalNorm = normalMatrix.MultT(finalNorm);
					finalNorm.Normalize();
					v.n.x = finalNorm.mData[0];
					v.n.y = finalNorm.mData[2];
					v.n.z = finalNorm.mData[1];
				}

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

				actor->m_vVertexList.emplace_back(v);
			}
		}
		iBasePolyIndex += iPolySize;
	}
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
		m_FbxNodes.emplace_back(pNode);
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

FbxColor SFbxImporter::ReadColor(FbxMesh* mesh,
	DWORD dwVertexColorCount,
	FbxLayerElementVertexColor* pVertexColorSet,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
	{
		switch (pVertexColorSet->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (pVertexColorSet->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
				Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}


FbxVector4 SFbxImporter::ReadNormal(const FbxMesh* mesh,
	DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
	int controlPointIndex, int iVertexIndex)
{
	FbxVector4 result(0, 0, 0);
	if (dwVertexNormalCount < 1)
	{
		return result;
	}
	int iVertexNormalLayer = mesh->GetElementNormalCount();

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// �븻 ȹ�� 

	// �븻 ���͸� ������ ���� 
	switch (VertexNormalSets->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (VertexNormalSets->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = VertexNormalSets->GetIndexArray().GetAt(controlPointIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = VertexNormalSets->GetIndexArray().GetAt(iVertexIndex);
			// �ε����� ���´�. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}