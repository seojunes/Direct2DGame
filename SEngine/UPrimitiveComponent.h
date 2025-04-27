#pragma once
#include "USceneComponent.h"
#include "UMaterial.h"
class UPrimitiveComponent : public USceneComponent
{
public:
	bool 		m_bRenderMesh = true;
	//m_SubChilds == 0 -> submaterial ºñ»ç¿ë
	std::vector<std::shared_ptr<UPrimitiveComponent>>    m_SubChilds;
	Matrix		m_matBoneAnim;
	std::vector<std::wstring>		m_csTextures;
	std::shared_ptr<UMaterial>		m_pMaterial;
	UPrimitiveComponent* m_pParent = nullptr;
public:
	virtual void  SetMaterial(std::shared_ptr<UMaterial> pMaterial);
public:
	ComPtr<ID3D11Buffer>		m_pVertexBuffer = nullptr;
	std::vector<PNCT_VERTEX>	m_vVertexList;
	ComPtr<ID3D11Buffer>		m_pVertexIWBuffer = nullptr;
	std::vector<IW_VERTEX>		m_vIWList;

	ComPtr<ID3D11Buffer>		m_pIndexBuffer = nullptr;
	std::vector<DWORD>			m_vIndexList;

	UINT						m_iIndex = 0;
	std::map<std::wstring, UINT>  m_FbxNodeNames;
	std::map<UINT, std::wstring>  m_FbxNameNodes;
	std::vector<std::wstring>		m_szNames;
	std::map<std::wstring, std::wstring>  m_FbxParentNameNodes;
	std::vector<Matrix>		m_AnimList;
	std::vector<Matrix>	    m_matBindPose;
	std::vector<UINT>			m_matID;
	//ComPtr<ID3D11Buffer>        m_pConstantBuffer = nullptr;
public:
	bool	CreateVertexBuffer();
	bool	CreateIndexBuffer();
public:
	virtual void   Init();
	virtual void   Tick();
	virtual void   PreRender();
	virtual void   Render();
	virtual void   PostRender();
	virtual void   Destroy();
};

