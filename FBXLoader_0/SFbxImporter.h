#pragma once
#include "AActor.h"
#include <fbxsdk.h>
//libfbxsdk - md.lib
//libxml2 - md.lib
//zlib - md.lib
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")


class SFbxNode
{
	std::wstring                m_szName;
	std::wstring                m_szTexName;
	std::vector<PNCT_VERTEX>	m_vVertexList;
	std::vector<DWORD>			m_vIndexList;
	std::vector<SFbxNode>	    m_Childs;
};
class SFbxFile
{
public:
	std::vector<SFbxNode>  m_lists;
};

class SFbxImporter
{
public:
	FbxManager* m_pManager;
	FbxImporter* m_pImporter;
	FbxScene* m_pScene;
	FbxNode* m_pRootNode;
	std::vector<FbxMesh*>  m_FbxMeshs;
public:
	bool  Load(std::string loadfile, AActor* actor);
	void  PreProcess(FbxNode* pNode);
	void  ParseMesh(
		FbxMesh* fbxmesh, AActor* actor);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
		int vertexIndex, int uvIndex, FbxVector2& uv);
	void  Destroy();
};

