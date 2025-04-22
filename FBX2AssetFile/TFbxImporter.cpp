#include "TFbxImporter.h"
void  TFbxImporter::reset()
{
	Destroy();
}
bool    TFbxImporter::ParseMeshSkinning(FbxMesh* fbxmesh,
	UPrimitiveComponent* actor)
{
	m_VertexWeights.clear();
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
TMatrix     TFbxImporter::DxConvertMatrix(TMatrix m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;


	TVector3 v0, v1, v2, v3;
	v0 = { mat.m[0][0], mat.m[0][1], mat.m[0][2] };
	v1 = { mat.m[1][0], mat.m[1][1], mat.m[1][2] };
	v2 = { mat.m[2][0], mat.m[2][1], mat.m[2][2] };
	v3 = v1 ^ v2;// D3DXVec3Cross(&v3, &v1, &v2);
	if ((v3 | v0) < 0.0f)
	{
		TMatrix matNegative;
		matNegative.Scale(-1.0f, -1.0f, -1.0f);
		mat = mat * matNegative;
	}

	return mat;
}
TMatrix     TFbxImporter::ConvertAMatrix(FbxAMatrix& m)
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
bool  TFbxImporter::Load(std::string loadfile, AAsset* actor)
{
	actor->m_szFileName = to_mw(loadfile);
	m_pManager = FbxManager::Create();
	m_pImporter = FbxImporter::Create(m_pManager, "");
	m_pScene = FbxScene::Create(m_pManager, "");

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

	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);
	m_pRootNode = m_pScene->GetRootNode();
	auto tFbxNodeRoot = std::make_shared<TFbxNodeTree>(m_pRootNode);
	tFbxNodeRoot->m_szName = to_mw(m_pRootNode->GetName());
	tFbxNodeRoot->m_szParentName = L"none";
	PreProcess(tFbxNodeRoot);

	GetAnimation();
	actor->m_iStartFrame = m_iStartFrame;
	actor->m_iEndFrame = m_iEndFrame;
	{
		actor->m_Header.iLastFrame = m_iEndFrame;
		actor->m_Header.iStartFrame = m_iStartFrame;
		actor->m_expFbxNodes.resize(m_FbxNodes.size());
		for( int i=0; i < m_FbxNodes.size();i ++)
		{
			actor->m_expFbxNodes[i].m_bMesh  = m_FbxNodes[i]->m_bMesh;
			_tcscpy_s(
				actor->m_expFbxNodes[i].m_szName,
				32,
				m_FbxNodes[i]->m_szName.c_str()	);
			_tcscpy_s(actor->m_expFbxNodes[i].m_szParentName,
				32,
				m_FbxNodes[i]->m_szParentName.c_str());
		}
	}

	auto mesh = std::make_shared<UStaticMeshComponent>();
	// 케릭터 당 m_matBindPose 행렬리스트
	//mesh->m_matBindPose.resize(m_FbxNodes.size());
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
			// 메쉬 당 m_matBindPose 행렬리스트
			ParseMesh(mesh, child.get());
		}
		child->m_szName = node->m_szName;
		for (int i = 0; mesh->m_Childs.size(); i++)
		{
			if (mesh->m_Childs[i]->m_szName == node->m_szParentName)
			{
				child->m_pParent = mesh->m_Childs[i].get();
				break;
			}
		}
		GetNodeAnimation(node->m_pFbxNode, child.get());
		mesh->m_Childs.emplace_back(child);
	}
	mesh->m_FbxNodeNames = m_FbxNodeNames;
	mesh->m_FbxNameNodes = m_FbxNameNodes;
	mesh->m_FbxParentNameNodes = m_FbxParentNameNodes;
	actor->SetMesh(mesh);

	Destroy();
	return true;
}
void        TFbxImporter::GetAnimation()
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
void        TFbxImporter::GetNodeAnimation(
	FbxNode* node,
	UPrimitiveComponent* actor)
{
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxTime time;
	for (FbxLongLong t = m_iStartFrame; t <= m_iEndFrame; t++)
	{
		time.SetFrame(t, TimeMode);
		FbxAMatrix matGlobal = node->EvaluateGlobalTransform(time);
		FbxNode* pParent = node->GetParent();
		FbxAMatrix matParent;
		if (pParent)
		{
			matParent = pParent->EvaluateGlobalTransform(time);
		}
		FbxAMatrix matLocal = matParent.Inverse() * matGlobal;
		FbxAMatrix matWorld = matParent * matLocal;
		TMatrix mat = DxConvertMatrix(ConvertAMatrix(matLocal));;
		actor->m_AnimList.push_back(mat);
	}
}
void TFbxImporter::ParseMesh(FbxMesh* fbxmesh,
	UPrimitiveComponent* actor)
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
	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	//GetAnimation(pNode, actor);

	// 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)
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

	/// texture filename
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
			std::string texName = ParseMaterial(pSurface);
			actor->m_csTextures.emplace_back(to_mw(texName));
		}
	}

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
void  TFbxImporter::PreProcess(tFbxTree& pParentNode)
{
	if (pParentNode == nullptr) return;
	FbxNode* node = pParentNode->m_pFbxNode;
	FbxMesh* pMesh = node->GetMesh();
	pParentNode->m_bMesh = false;
	if (pMesh != nullptr)
	{
		m_FbxMeshs.emplace_back(pMesh);
		pParentNode->m_bMesh = true;
	}
	pParentNode->m_szName = to_mw(node->GetName());
	
	m_FbxNodes.emplace_back(pParentNode);
	m_FbxNodeNames.insert(std::make_pair(to_mw(node->GetName()),
		m_FbxNodeNames.size()));
	m_FbxNameNodes.insert(std::make_pair(m_FbxNodeNames.size() - 1,
		to_mw(node->GetName())));

	m_FbxParentNameNodes.insert(std::make_pair(to_mw(node->GetName()), pParentNode->m_szParentName));
	int iNumChild = node->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChild = node->GetChild(iNode);
		auto tFbxChildTree = std::make_shared<TFbxNodeTree>(pChild);
		tFbxChildTree->m_szName = to_mw(pChild->GetName());
		tFbxChildTree->m_pFbxParentNode = node;
		pParentNode->m_Childs.emplace_back(tFbxChildTree);
		tFbxChildTree->m_szParentName = pParentNode->m_szName;		
		PreProcess(tFbxChildTree);
	}
}
void TFbxImporter::Destroy()
{
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
	m_FbxNameNodes.clear();
}

void TFbxImporter::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
	int vertexIndex, int uvIndex, FbxVector2& uv)
{
	FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
	if (pFbxLayerElementUV == nullptr) {
		return;
	}
	// 매핑방식 뭐냐?// enum EMappingMode
	/*{
		eNone,	eByControlPoint,eByPolygonVertex,	eByPolygon,	eByEdge,eAllSame
	};*/
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
FbxColor TFbxImporter::ReadColor(FbxMesh* mesh,
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


FbxVector4 TFbxImporter::ReadNormal(const FbxMesh* mesh,
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


std::string TFbxImporter::ParseMaterial(FbxSurfaceMaterial* pSurface)
{
	std::vector<std::string> surfacelist =
	{
		"FbxSurfaceMaterial::sShadingModel",
		"FbxSurfaceMaterial::sMultiLayer",
		"FbxSurfaceMaterial::sEmissive",
		"FbxSurfaceMaterial::sEmissiveFactor",
		"FbxSurfaceMaterial::sAmbient",
		"FbxSurfaceMaterial::sAmbientFactor",
		"FbxSurfaceMaterial::sDiffuse",
		"FbxSurfaceMaterial::sDiffuseFactor",
		"FbxSurfaceMaterial::sSpecular",
		"FbxSurfaceMaterial::sSpecularFactor",
		"FbxSurfaceMaterial::sShininess",
		"FbxSurfaceMaterial::sBump",
		"FbxSurfaceMaterial::sNormalMap",
		"FbxSurfaceMaterial::sBumpFactor",
		"FbxSurfaceMaterial::sTransparentColor",
		"FbxSurfaceMaterial::sTransparencyFactor",
		"FbxSurfaceMaterial::sReflection",
		"FbxSurfaceMaterial::sReflectionFactor",
		"FbxSurfaceMaterial::sDisplacementColor",
		"FbxSurfaceMaterial::sDisplacementFactor",
		"FbxSurfaceMaterial::sVectorDisplacementColor",
		"FbxSurfaceMaterial::sVectorDisplacementFactor",
	};
	for (auto& str : surfacelist)
	{
		FbxProperty prop = pSurface->FindProperty(str.c_str());
		if (prop.IsValid())
		{
			FbxString name = prop.GetName();
			FbxDataType type = prop.GetPropertyDataType();
			FbxDouble3 value = prop.Get<FbxDouble3>();
			FbxDouble4 value4 = prop.Get<FbxDouble4>();
			FbxDouble value1 = prop.Get<FbxDouble>();
			FbxBool value2 = prop.Get<FbxBool>();

			FbxFileTexture* texfile = prop.GetSrcObject<FbxFileTexture>(0);
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
	}
	//auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
	//if (property.IsValid())
	//{
	//	//std::string texName;
	//	FbxFileTexture* texfile =
	//		property.GetSrcObject<FbxFileTexture>(0);
	//	if (texfile)
	//	{
	//		const char* szTexPath = texfile->GetFileName();
	//		CHAR Drive[MAX_PATH];
	//		CHAR Dir[MAX_PATH];
	//		CHAR FName[MAX_PATH];
	//		CHAR Ext[MAX_PATH];
	//		_splitpath_s(szTexPath, Drive, Dir, FName, Ext);
	//		std::string texName = FName;
	//		std::string ext = Ext;
	//		if (ext == ".tga" || ext == ".TGA")
	//		{
	//			ext.clear();
	//			ext = ".dds";
	//		}
	//		texName += ext;
	//		return texName;
	//	}
	//}

	return std::string();
}

int TFbxImporter::GetSubMaterialIndex(
	int iPoly, FbxLayerElementMaterial* pMaterialSetList)
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