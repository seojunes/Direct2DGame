#include "SFbxImporter.h"

bool    SFbxImporter::ParseMeshSkinning(FbxMesh* fbxmesh, UPrimitiveComponent* actor)
{
	m_VertexWeights.clear();
	//스키닝인지 아닌지 체크.
	int iDeformerCount = fbxmesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount <= 0) return false;
	// 중요 : 메쉬에 정점 개수와 iVretexCount는 같다.
	int iVertexCount = fbxmesh->GetControlPointsCount();
	m_VertexWeights.resize(iVertexCount);

	for (int iDeformer = 0; iDeformer < iDeformerCount; iDeformer++)
	{
		FbxSkin* pSkin = (FbxSkin*)fbxmesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		if (pSkin == nullptr) continue;
		int iClusterCount = pSkin->GetClusterCount();
		for (int iCluster = 0; iCluster < iClusterCount; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			if (pCluster == nullptr) continue;
			FbxNode* pLinkNode = pCluster->GetLink();
			if (pLinkNode == nullptr) continue;
			auto iter = m_FbxNodeNames.find(to_mw(pLinkNode->GetName()));
			UINT iWeightIndex = 0;
			if (iter != m_FbxNodeNames.end())
			{
				iWeightIndex = iter->second;
			}

			/// 본 오브젝트의 좌표계 변환 행렬
			FbxAMatrix matXBindPosLink;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPosLink);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxAMatrix matWorldBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPosLink;
			FbxAMatrix matBindPose = matWorldBindPose.Inverse(); // 본의 로컬 좌표계로 변환
			TMatrix mat = DxConvertMatrix(ConvertAMatrix(matBindPose));
			actor->m_matBindPose.emplace_back(mat);
			actor->m_matID.emplace_back(iWeightIndex);
			actor->m_szNames.emplace_back(to_mw(pLinkNode->GetName()));


			int iClusterSize = pCluster->GetControlPointIndicesCount();
			int* pFbxNodeIndex = pCluster->GetControlPointIndices();
			double* pFbxNodeWegiht = pCluster->GetControlPointWeights();
			for (int v = 0; v < iClusterSize; v++)
			{
				int iIndex = pFbxNodeIndex[v];
				float fWeight = pFbxNodeWegiht[v];
				int iMaxCnt = m_VertexWeights[iIndex].Insert(iWeightIndex, fWeight);
				if (m_iMaxWeightCount < iMaxCnt)
				{
					m_iMaxWeightCount = iMaxCnt;
				}
			}
		}
	}
	return true;
}

//FBX는 기본적으로, Y-up, Right-handed 좌표계인데
//DX는 Y-up , Left-handed좌표계를 이용하므로 좌표계 변환.
TMatrix     SFbxImporter::DxConvertMatrix(TMatrix m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}

//FbxAMatrix를 TMatrix로 변환 (double -> float)
//행렬의 값은 동일하지만, 타입만 변환되고, 좌표계변환은 일어나지 않음.
TMatrix     SFbxImporter::ConvertAMatrix(FbxAMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}

void SFbxImporter::Reset()
{
	Destroy();
}

// FBX파일을 로드하고, 씬 그래프와 메시, 애니메이션 등을 처리해서 
// AActor 객체에 설정해주는 전체 파이프라인의 핵심
// 전체 역할 요약 : FBX Manager 생성 -> 파일 Import -> 씬 전처리 -> 메시/애니메이션 파싱 -> AActor에 세팅
bool  SFbxImporter::Load(std::string loadfile, AActor* actor)
{
	// FBX SDK 기본 객체 초기화
	m_pManager = FbxManager::Create();						//FBX라이브러리 전체 관리 객체
	m_pImporter = FbxImporter::Create(m_pManager, "");		//파일로더
	m_pScene = FbxScene::Create(m_pManager, "");			//씬 그래프 (노드 트리 구조)

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

	//좌표계 보정 Maya Z-Up 좌표계로 변환.
	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);
	// Scene의 루트를 받아서
	m_pRootNode = m_pScene->GetRootNode();
	auto sFbxNodeRoot = std::make_shared<SFbxNodeTree>(m_pRootNode);
	PreProcess(sFbxNodeRoot);

	GetAnimation();
	actor->m_iStartFrame = m_iStartFrame;
	actor->m_iEndFrame = m_iEndFrame;
	//UStaticMeshComponent : fbxfile
	//nodes[10] UPrimitiveComponent

	/*auto mesh = std::make_shared<UStaticMeshComponent>();
	for (int iMesh = 0; iMesh < m_FbxMeshs.size(); iMesh++)
	{
		auto child = std::make_shared<UPrimitiveComponent>();
		ParseMesh(m_FbxMeshs[iMesh], child.get());
		mesh->m_Childs.emplace_back(child);
	}
	actor->SetMesh(mesh);*/
	//메시 및 애니메이션 파싱
	auto mesh = std::make_shared<UStaticMeshComponent>();
	// 케릭터 당 m_matBindPose 행렬리스트
	mesh->m_matBindPose.resize(m_FbxNodes.size());
	for (int iNode = 0; iNode < m_FbxNodes.size(); iNode++)
	{
		auto node = m_FbxNodes[iNode];
		auto child = std::make_shared<UPrimitiveComponent>();
		node->m_iIndex = child->m_iIndex = iNode;
		child->m_bRenderMesh = false;
		if (node->m_bMesh)
		{
			child->m_bRenderMesh = true;
			auto mesh = node->m_pFbxNode->GetMesh();
			//메쉬 당 m_matBindPose 행렬리스트.
			ParseMesh(mesh, child.get());
		}
		child->m_szName = node->m_szName;

		GetNodeAnimation(node->m_pFbxNode, child.get());
		mesh->m_Childs.emplace_back(child);
	}
	mesh->m_FbxNodeNames = m_FbxNodeNames;
	mesh->m_FbxNameNodes = m_FbxNameNodes;
	actor->SetMesh(mesh);

	Destroy();
	return true;
}


/// <summary>
/*[1] 기하학적 변환 행렬 구함
[2] 레이어에서 UV / 노멀 / 컬러 / 머티리얼 정보 추출
[3] 텍스처 이름 추출
[4] 서브머티리얼 존재 시 서브 메시 객체 생성
[5] 각 폴리곤 순회하며 정점 3개씩 추출(삼각형 분해)
[6] 정점 데이터 구성 : 위치, 컬러, 노멀, UV
[7] 머티리얼 인덱스 기반으로 VertexList에 삽입*/
/// </summary>
void SFbxImporter::ParseMesh(FbxMesh* fbxmesh, UPrimitiveComponent* actor)
{
	bool bSkinned = ParseMeshSkinning(fbxmesh, actor);
	/// 기하행렬(초기 정점 위치를 변환 할 때 사용)
	FbxNode* pNode = fbxmesh->GetNode();
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	// 노멀 변환용 행렬
	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	// 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)
	//UV, 컬러 , 노멀, 머터리얼을 모든 레이어에서 수집.
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
		if (pFbxLayer->GetNormals() != NULL)
		{
			VertexNormalSets.push_back(pFbxLayer->GetNormals());
		}
		if (pFbxLayer->GetMaterials() != nullptr)
		{
			MaterialSet.push_back(pFbxLayer->GetMaterials());
		}
	}

	/// texture filename 추출 + 서브 메시 구조 준비.
	int iNumMtl = pNode->GetMaterialCount();
	if (iNumMtl > 1)
	{
		actor->m_SubChilds.resize(iNumMtl);
		for (int iMtrl = 0; iMtrl < iNumMtl; iMtrl++)
		{
			actor->m_SubChilds[iMtrl] = std::make_shared<UPrimitiveComponent>();
		}
	}

	for (int iMtrl = 0; iMtrl < iNumMtl; iMtrl++)
	{
		FbxSurfaceMaterial* pSurface = pNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			//ParseMaterial을 통해서 텍스처 이름을 가져오고 actor의 m_csTextures에 저장.
			std::string texName = ParseMaterial(pSurface);
			actor->m_csTextures.emplace_back(to_mw(texName));
		}
	}

	//폴리곤 루프.
	int iNumPolyCount = fbxmesh->GetPolygonCount();
	FbxVector4* pVertexPositions = fbxmesh->GetControlPoints();
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = fbxmesh->GetPolygonSize(iPoly);
		// 폴리곤 3 or 4 
		int iNumFace = iPolySize - 2;
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			int VertexIndex[3] = { 0, iFace + 2, iFace + 1 };
			// INDEX -> MAX 0, 2, 1   : 오른손 좌표계
			// INDEX -> DX  0, 1, 2   : 왼손 좌표계
			// 정점위치 인덱스
			int CornerIndex[3];
			CornerIndex[0] = fbxmesh->GetPolygonVertex(iPoly, VertexIndex[0]);
			CornerIndex[1] = fbxmesh->GetPolygonVertex(iPoly, VertexIndex[1]);
			CornerIndex[2] = fbxmesh->GetPolygonVertex(iPoly, VertexIndex[2]);

			//텍스처좌표 인덱스
			int uvIndex[3];
			uvIndex[0] = fbxmesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
			uvIndex[1] = fbxmesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
			uvIndex[2] = fbxmesh->GetTextureUVIndex(iPoly, VertexIndex[2]);

			for (int index = 0; index < 3; index++)
			{
				PNCT_VERTEX v;
				FbxVector4 p = geom.MultT(pVertexPositions[CornerIndex[index]]);
				//FbxVector4 p = pVertexPositions[CornerIndex[index]];
				// 정점좌표계 
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

				//UV좌표 처리.
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

					// MAX  텍스처좌표계   U,V
					// DX       ""         U, 1.0f-V  
					v.t.x = uv.mData[0];
					v.t.y = 1.0f - uv.mData[1];
				}

				int iSubMateriaIndex = 0;
				if (MaterialSet.size() > 0)
				{
					iSubMateriaIndex = GetSubMaterialIndex(iPoly, MaterialSet[0]);
				}
				IW_VERTEX iw;
				iw.i1[0] = actor->m_iIndex;
				iw.w1[0] = 1.0f;
				if (bSkinned)
				{
					TVertexWeight& weight = m_VertexWeights[CornerIndex[index]];
					for (int i = 0; i < weight.m_iCounter; i++)
					{
						if (i >= 4)
						{
							iw.i2[i - 4] = weight.m_iIndex[i];
							iw.w2[i - 4] = weight.m_fWeight[i];
						}
						else
						{
							iw.i1[i] = weight.m_iIndex[i];
							iw.w1[i] = weight.m_fWeight[i];
						}
					}
				}
				if (actor->m_SubChilds.size() <= 0)
				{
					actor->m_vVertexList.emplace_back(v);
					actor->m_vIWList.emplace_back(iw);
				}
				else
				{
					actor->m_SubChilds[iSubMateriaIndex]->m_vVertexList.emplace_back(v);
					actor->m_SubChilds[iSubMateriaIndex]->m_vIWList.emplace_back(iw);
				}
			}
		}


		iBasePolyIndex += iPolySize;
	}
}
// 재귀함수로 모든 MESH를 찾아서 FbxMeshs에 저장함.
void  SFbxImporter::PreProcess(sFbxTree& pParentNode)
{
	if (pParentNode == nullptr) return;
	//현재 노드에서 Mesh체크.
	FbxNode* node = pParentNode->m_pFbxNode;
	FbxMesh* pMesh = node->GetMesh();
	pParentNode->m_bMesh = false;
	if (pMesh != nullptr)
	{
		m_FbxMeshs.emplace_back(pMesh);
		pParentNode->m_bMesh = true;
	}
	m_FbxNodes.emplace_back(pParentNode);
	m_FbxNodeNames.insert(std::make_pair(to_mw(node->GetName()),
		m_FbxNodeNames.size()));
	m_FbxNameNodes.insert(std::make_pair(m_FbxNodeNames.size() - 1,
		to_mw(node->GetName())));

	int iNumChild = node->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChild = node->GetChild(iNode);
		// 헬퍼오브젝트 + 지오메트리 오브젝트
	//if (pChild->GetNodeAttribute() != nullptr)
	//{
	//	FbxNodeAttribute::EType type = pChild->GetNodeAttribute()->GetAttributeType();
	//	if (/*type == FbxNodeAttribute::eMesh ||*/
	//		type == FbxNodeAttribute::eSkeleton 
	//		/*type == FbxNodeAttribute::eNull*/)
	//	{
	//		auto tFbxChildTree = std::make_shared<TFbxNodeTree>(pChild);
	//		tFbxChildTree->m_szName = to_mw(pChild->GetName());
	//		tFbxChildTree->m_pFbxParentNode = node;
	//		pParentNode->m_Childs.emplace_back(tFbxChildTree);
	//		tFbxChildTree->m_szParentName = pParentNode->m_szName;
	//		PreProcess(tFbxChildTree);
	//	}
	//}else
	//{
	//	auto tFbxChildTree = std::make_shared<TFbxNodeTree>(pChild);
	//	tFbxChildTree->m_szName = to_mw(pChild->GetName());
	//	tFbxChildTree->m_pFbxParentNode = node;
	//	pParentNode->m_Childs.emplace_back(tFbxChildTree);
	//	tFbxChildTree->m_szParentName = pParentNode->m_szName;
	//	PreProcess(tFbxChildTree);			
	//}
		auto sFbxChildTree = std::make_shared<SFbxNodeTree>(pChild);
		sFbxChildTree->m_szName = to_mw(pChild->GetName());
		sFbxChildTree->m_pFbxParentNode = node;
		pParentNode->m_Childs.emplace_back(sFbxChildTree);
		sFbxChildTree->m_szParentName = pParentNode->m_szName;
		PreProcess(sFbxChildTree);
	}
}
// 메모리 해제부분
void SFbxImporter::Destroy()
{
	//FBX내부 시스템 객체는 스마트 포인터를 쓸수없는 내부 객체이기 때문에 직접 해제를 해줘야된다.
	if (m_pScene)	m_pScene->Destroy();
	if (m_pImporter) m_pImporter->Destroy();
	if (m_pManager)  m_pManager->Destroy();
	m_pScene = nullptr;
	m_pImporter = nullptr;
	m_pManager = nullptr;

	m_FbxMeshs.clear();
	m_FbxNodes.clear();
	m_matBindPose.clear();
	m_FbxNodeNames.clear();
}
// FBX Mesh 객체에서 텍스처좌표 (UV정보)를 추출하는 함수
void SFbxImporter::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
	int vertexIndex, int uvIndex, FbxVector2& uv)
{
	//UV set 유효성 검사.
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
		case FbxLayerElementUV::eDirect: // fbx 5.0 이하일 때
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

//FBX 메시에서 정점하나의 컬러(RGBA)를 정확하게 읽어오는 함수.
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

//FBX메시에서 하나의 정점 노멀(FbxVector4 타입)을 정확하게 추출하는 함수.
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
	// 노말 획득 

	// 노말 벡터를 저장할 벡터 
	switch (VertexNormalSets->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
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
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
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
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}

//FBX Material에서 디퓨즈 텍스처 경로를 추출해서 사용 가능한 텍스처 파일명(ex .dds)만 리턴
std::string SFbxImporter::ParseMaterial(FbxSurfaceMaterial* pSurface)
{
	auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (property.IsValid())
	{
		//std::string texName;
		FbxFileTexture* texfile =
			property.GetSrcObject<FbxFileTexture>(0);
		if (texfile)
		{
			const char* szTexPath = texfile->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath_s(szTexPath, Drive, Dir, FName, Ext);
			std::string texName = FName;
			std::string ext = Ext;
			if (ext == ".tga" || ext == ".TGA")
			{
				ext.clear();
				ext = ".dds";
			}
			texName += ext;
			return texName;
		}
	}

	return std::string();
}

//sub
//해당 폴리곤의 정점들을 어느 SubMesh에 넣을지 판단.
int SFbxImporter::GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* pMaterialSetList)
{
	int iSubMtrl = 0;
	if (pMaterialSetList != nullptr)
	{
		switch (pMaterialSetList->GetMappingMode())
		{
		case FbxLayerElement::eByPolygon:
		{
			// 매핑 정보가 배열에 저장되는 방식
			switch (pMaterialSetList->GetReferenceMode())
			{
			case FbxLayerElement::eIndex:
			{
				iSubMtrl = iPoly;
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				iSubMtrl = pMaterialSetList->GetIndexArray().GetAt(iPoly);
			}break;
			}
		}
		default:
		{
			break;
		}
		}
	}
	return iSubMtrl;
}

// FBX에서 노드의 애니메이션 트랜스폼 행렬을 추출해서 UPrimitiveComponent에 저장해주는 함수.
// 노드의 각 프레임별 트랜스폼 행렬을 계싼해서 actor->m_AnimList에 프레임별 행렬을 순서대로 저장
void        SFbxImporter::GetAnimation()
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pScene->GetSrcObject<FbxAnimStack>(0);
	if (stack == nullptr) return;

	FbxString TakeName = stack->GetName();
	FbxTakeInfo* TakeInfo = m_pScene->GetTakeInfo(TakeName);
	FbxTimeSpan LocalTimeSpan = TakeInfo->mLocalTimeSpan;
	FbxTime start = LocalTimeSpan.GetStart();
	FbxTime end = LocalTimeSpan.GetStop();
	FbxTime Duration = LocalTimeSpan.GetDuration();

	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	m_iStartFrame = start.GetFrameCount(TimeMode);
	m_iEndFrame = end.GetFrameCount(TimeMode);
}
void        SFbxImporter::GetNodeAnimation(
	FbxNode* node,
	UPrimitiveComponent* actor)
{
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxTime time;
	for (FbxLongLong t = m_iStartFrame; t <= m_iEndFrame; t++)
	{
		time.SetFrame(t, TimeMode);
		FbxAMatrix matGlobal = node->EvaluateGlobalTransform(time);
		TMatrix mat = DxConvertMatrix(ConvertAMatrix(matGlobal));;
		actor->m_AnimList.push_back(mat);
	}
}
