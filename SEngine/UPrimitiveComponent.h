#pragma once
#include "USceneComponent.h"
class UPrimitiveComponent : public USceneComponent
{
public:
	ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	std::vector<PNCT_VERTEX>	m_vVertexList;
	std::vector<DWORD>			m_vIndexList;
public:
	bool	CreateVertexBuffer();
	bool	CreateIndexBuffer();
public:
	virtual void   Init();
	virtual void   Tick();
	virtual void   Render();
	virtual void   Destroy();
};

