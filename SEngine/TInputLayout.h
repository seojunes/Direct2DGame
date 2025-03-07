#pragma once
#include "TShader.h"
class TInputLayout
{
	ComPtr< ID3D11InputLayout> m_pInputLayout = nullptr;
public:
	std::wstring   m_csName;	
	virtual  bool  Load(ID3DBlob* pCode,
						D3D11_INPUT_ELEMENT_DESC layout[],
						UINT szNumCounter);
	ID3D11InputLayout* Get();
	virtual  void  Release();
	TInputLayout(std::wstring key) : m_csName(key)
	{

	}
	TInputLayout() = default;
};
class TTInputLayoutManager
{
public:
	static TInputLayout* g_pInputLayout;
	// ΩÃ±€≈Ê ∆–≈œ
	static TTInputLayoutManager& GetInstance()
	{
		static TTInputLayoutManager mgr;
		return mgr;
	}
	TInputLayout* Load(ID3DBlob* pCode, std::wstring key);
	TInputLayout* Load( ID3DBlob* pCode,
							D3D11_INPUT_ELEMENT_DESC layout[],
							UINT szNumCounter,
							std::wstring key);
	TInputLayout* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Init(ID3DBlob* pCode);
private:
	std::map<std::wstring, TInputLayout*> maplist;
	TTInputLayoutManager() {}
public:
	~TTInputLayoutManager();
};
#define I_InputLayout TTInputLayoutManager::GetInstance()