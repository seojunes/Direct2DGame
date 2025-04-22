#pragma once
#include "DxState.h"
#include "Texture.h"
#include "InputLayout.h"
struct TLoadResData
{
	std::wstring texPathName;
	std::wstring texShaderName;
	std::wstring texStateList[4];
};
class MeshRender
{
public:
	ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	Shader* m_pShader = nullptr;
	Texture* m_pTexture = nullptr;
	InputLayout* m_pInputLayout = nullptr;
	std::vector<TVector2>		m_vScreenList;
	std::vector<PCT_VERTEX>		m_vVertexList;
	std::vector<DWORD>			m_vIndexList;
	TLoadResData				m_LoadResData;
public:
	virtual bool SetShader(Shader* pShader = nullptr);
	virtual bool SetTexture(Texture*);
	virtual bool SetLayout(InputLayout* pInputLayout = nullptr);
	virtual bool	Create();
	virtual bool	Create(TLoadResData data);
	virtual bool	CreateVertexBuffer(); // 持失
	virtual bool	CreateIndexBuffer(); // 持失
	virtual bool	CreateVertexShader(); // 持失
	virtual bool	CreatePixelShader(); // 持失
	virtual bool	CreateInputLayout(); // 持失
	virtual void    SetVertexData();
	virtual void    SetIndexData();
	virtual bool    LoadTexture(std::wstring texName);
public:
	virtual void    Transform(TVector2 vCamera);
	virtual void	PreRender();
	virtual void	Render();
	virtual void	PostRender();
	virtual void	Release();
public:
	MeshRender();
	virtual ~MeshRender();
};
