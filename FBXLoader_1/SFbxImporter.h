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

struct SFbxNodeTree
{
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

	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
public:
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

	void    GetAnimation(FbxNode* node, UPrimitiveComponent* actor);
};

