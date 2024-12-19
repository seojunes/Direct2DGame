#pragma once
#include "TCollision.h"
#include "TDxState.h"
#include "TSound.h"
#include "TTexture.h"
#include "TInputLayout.h"
struct TLoadResData
{
	std::wstring texPathName;
	std::wstring texShaderName;

};
class TObject
{
public:
	ComPtr<ID3D11Buffer> m_pVertexBuffer=nullptr;	
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	TShader*			m_pShader = nullptr;	
	//TShader*			m_pPixelShader = nullptr;
	TTexture*			m_pTexture = nullptr;
	TInputLayout*		m_pInputLayout = nullptr;
	std::vector<TVertex2>		m_vScreenList;
	std::vector<PCT_VERTEX>		m_vVertexList;
	std::vector<DWORD>			m_vIndexList;
	TRect						m_srtScreen;
	TLoadResData				m_LoadResData;
public:
	TObject& SetShader(TShader* pShader= nullptr);
	TObject& SetTexture(TTexture* );
	TObject& SetLayout(TInputLayout* pInputLayout=nullptr);
	//virtual bool SetLayout( TShader* pShader,
	//						g_VertexLayout[0]);
	virtual bool	Create();   
	virtual bool	Create(TLoadResData data);
	virtual bool	Create(TLoadResData data,
		TVertex2 s,		// ȭ����ǥ ����
		TVertex2 t) ;   // ȭ����ǥ ��
		virtual bool	CreateVertexBuffer(); // ����
		virtual bool	CreateIndexBuffer(); // ����
		virtual bool	CreateVertexShader(); // ����
		virtual bool	CreatePixelShader(); // ����
		virtual bool	CreateInputLayout(); // ����
	virtual void    SetVertexData();
	virtual void    SetIndexData();
	virtual bool    LoadTexrture(std::wstring texName);
public:
	virtual void	Init();
	virtual void	Frame();
	virtual void    Transform(TVertex2 vCamera);
	virtual void	PreRender();
	virtual void	Render();
	virtual void	PostRender();
	virtual void	Release();
public:
	TObject();
	virtual ~TObject();
};
