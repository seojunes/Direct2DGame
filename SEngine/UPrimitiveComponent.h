#pragma once
#include "USceneComponent.h"
#include "UMaterial.h"
class UPrimitiveComponent : public USceneComponent
{
public:
	//m_SubChilds == 0 -> submaterial ºñ»ç¿ë
	std::vector<std::shared_ptr<UPrimitiveComponent>>    m_SubChilds;

	std::vector<std::wstring>		m_csTextures;
	std::shared_ptr<UMaterial>		m_pMaterial;
public:
	virtual void  SetMaterial(std::shared_ptr<UMaterial> pMaterial);
public:
	ComPtr<ID3D11Buffer>		m_pVertexBuffer = nullptr;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer = nullptr;
	std::vector<PNCT_VERTEX>	m_vVertexList;
	std::vector<DWORD>			m_vIndexList;
	std::vector<TMatrix>		m_AnimList;
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

