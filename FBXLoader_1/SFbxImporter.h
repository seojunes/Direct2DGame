#pragma once
#include "AActor.h"
#include <fbxsdk.h>
//libfbxsdk - md.lib
//libxml2 - md.lib
//zlib - md.lib
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")
//struct 전방선언을 할 때, class로 선언해도 상관없음.
class SFbxNodeTree;
using sFbxTree = std::shared_ptr<SFbxNodeTree>;

struct TVertexWeight
{
	std::vector<UINT>		m_iIndex;
	std::vector<float>		m_fWeight;
	int  m_iCounter = 0;
	TVertexWeight()
	{
		m_iCounter = 0;
		m_iIndex.resize(8);
		m_fWeight.resize(8);
	}
	int Insert(UINT iIndex, float fWeight)
	{
		size_t i = 0;
		for (i = 0; i < m_iIndex.size(); ++i)
		{
			if (fWeight > m_fWeight[i])
			{
				for (size_t j = (m_iIndex.size() - 1); j > i; --j)
				{
					m_iIndex[j] = m_iIndex[j - 1];
					m_fWeight[j] = m_fWeight[j - 1];
				}
				m_iIndex[i] = iIndex;
				m_fWeight[i] = fWeight;
				m_iCounter++;
				break;
			}
		}
		return i;
	}
	/*int Insert(UINT iIndex, float fWeight)
	{
		m_iIndex.emplace_back(iIndex);
		m_fWeight.emplace_back(fWeight);
		return m_iIndex.size();
	};*/
};

struct SFbxNodeTree
{
	UINT						 m_iIndex;
	bool						 m_bMesh;
	std::wstring                 m_szName;
	std::wstring                 m_szParentName;
	FbxNode*					 m_pFbxParentNode;
	FbxNode*					 m_pFbxNode;
	std::vector<sFbxTree>		 m_Childs;
	SFbxNodeTree(FbxNode* node) { m_pFbxNode = node; }
};

class SFbxImporter
{
public:
	FbxManager* m_pManager;
	FbxImporter* m_pImporter;
	FbxScene* m_pScene;
	FbxNode* m_pRootNode;
	std::vector<FbxMesh*>  m_FbxMeshs;
	std::vector<sFbxTree>  m_FbxNodes;
	std::vector<TMatrix>   m_matBindPose;
	std::map<std::wstring, UINT>  m_FbxNodeNames;
	std::map<UINT, std::wstring>  m_FbxNameNodes;
	//std::map<FbxNode*, UINT>  m_FbxNodes;

	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
public:
	void  Reset();
	bool  Load(std::string loadfile, AActor* actor);
	void  PreProcess(sFbxTree& pParentNode);
	void  ParseMesh(FbxMesh* fbxmesh, UPrimitiveComponent* actor);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,	int vertexIndex, int uvIndex, FbxVector2& uv);
	FbxColor ReadColor(FbxMesh* mesh, DWORD dwVertexColorCount,
		FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int iVertexIndex);
	void  Destroy();

	std::string ParseMaterial(FbxSurfaceMaterial* pSurface);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial*);

public:
	FbxLongLong m_iStartFrame = 0;
	FbxLongLong m_iEndFrame = 0;
	std::vector< TVertexWeight>  m_VertexWeights;
	int m_iMaxWeightCount = 0;
	void    GetAnimation();
	void    GetNodeAnimation(FbxNode* node, UPrimitiveComponent* actor);
	bool    ParseMeshSkinning(FbxMesh* fbxmesh, UPrimitiveComponent* actor);
};

