#include "SFbxImporter.h"

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

bool  SFbxImporter::Load(std::string loadfile, AActor* actor)
{
	m_pManager = FbxManager::Create();					//FBX SDK의 전역관리자 (FBX 시스템 초기화)
	//FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);		// 파일 입출력 관리 
	//m_pManager->SetIOSettings(ios);

	m_pImporter = FbxImporter::Create(m_pManager, "");	// FBX파일을 로드하는 객체
	m_pScene = FbxScene::Create(m_pManager, "");		// 로드된 FBX파일의 장면(Scene) 정보를 저장하는 객체

	//FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);		//FBX가 쓰는 좌표계가 다를수 있어서, Maya Z-Up 좌표계로 변환. << 

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

	//FbxAxisSystem	 m_SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::MayaZUp.ConvertScene(m_pScene);
	//FbxSystemUnit	m_SceneSystemUnit = m_pScene->GetGlobalSettings().GetSystemUnit();

	//if (m_SceneSystemUnit.GetScaleFactor() != 1)	//if (m_SceneSystemUnit != FbxSystemUnit::cm)
	//{
	//	FbxSystemUnit::cm.ConvertScene(m_pScene);
	//}

	// vb, ib, texture, animation
	m_pRootNode = m_pScene->GetRootNode();			// 이 단계에서 모델을 가져옴
	PreProcess(m_pRootNode);		
	//
	actor->m_iStartFrame = m_iStartFrame;
	actor->m_iEndFrame = m_iEndFrame;

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
	/// 기하행렬(초기 정점 위치를 변환 할 때 사용)
	FbxNode* pNode = fbxmesh->GetNode();
	FbxAMatrix geom;
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

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
	FbxLongLong s = start.GetFrameCount(TimeMode);
	FbxLongLong n = end.GetFrameCount(TimeMode);
	FbxTime time;
	for (FbxLongLong t = s; t <= n; t++)
	{
		time.SetFrame(t, TimeMode);
		FbxAMatrix matGlobal = pNode->EvaluateGlobalTransform(time);
		TMatrix mat = DxConvertMatrix(ConvertAMatrix(matGlobal));;
		actor->m_AnimList.push_back(mat);
	}
	/*FbxLongLong t = 0;
	time.SetFrame(t, TimeMode);
	FbxAMatrix matGlobal = pNode->EvaluateGlobalTransform(time);
	if (matGlobal.IsIdentity()==false)
	{
		geom = matGlobal *geom;
	}*/
	FbxAMatrix normalMatrix = geom;

	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

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
				if (actor->m_SubChilds.size() <= 0)
					actor->m_vVertexList.emplace_back(v);
				else
					actor->m_SubChilds[iSubMateriaIndex]->m_vVertexList.emplace_back(v);
			}
		}


		iBasePolyIndex += iPolySize;
	}	
}
// 재귀함수로 모든 MESH를 찾아서 FbxMeshs에 저장함.
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

	m_FbxMeshs.clear();
	m_FbxNodes.clear();
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
int SFbxImporter::GetSubMaterialIndex(
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