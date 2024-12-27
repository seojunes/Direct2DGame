#pragma once
#include "TDxState.h"
#include "TTexture.h"
#include "TInputLayout.h"
struct TLoadResData
{
	std::wstring texPathName;
	std::wstring texShaderName;

};
class TMeshRender
{
public:
	ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	TShader* m_pShader = nullptr;
	TTexture* m_pTexture = nullptr;
	TInputLayout* m_pInputLayout = nullptr;
	std::vector<TVector2>		m_vScreenList;
	std::vector<PCT_VERTEX>		m_vVertexList;
	std::vector<DWORD>			m_vIndexList;
	TLoadResData				m_LoadResData;
public:
	virtual bool SetShader(TShader* pShader = nullptr);
	virtual bool SetTexture(TTexture*);
	virtual bool SetLayout(TInputLayout* pInputLayout = nullptr);
	virtual bool	Create();
	virtual bool	Create(TLoadResData data);
	virtual bool	Create(TLoadResData data,
		TVector2 s,		// 拳搁谅钎 矫累
		TVector2 t);   // 拳搁谅钎 场
	virtual bool	CreateVertexBuffer(); // 积己
	virtual bool	CreateIndexBuffer(); // 积己
	virtual bool	CreateVertexShader(); // 积己
	virtual bool	CreatePixelShader(); // 积己
	virtual bool	CreateInputLayout(); // 积己
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
	TMeshRender();
	virtual ~TMeshRender();
};
