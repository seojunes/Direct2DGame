#pragma once
#include "Shader.h"
class InputLayout
{
	ComPtr< ID3D11InputLayout> m_pInputLayout = nullptr;
public:
	std::wstring   m_csName;
	virtual  bool  Load(ID3DBlob* pCode,
		D3D11_INPUT_ELEMENT_DESC layout[],
		UINT szNumCounter);
	ID3D11InputLayout* Get();
	virtual  void  Release();
	InputLayout(std::wstring key) : m_csName(key)
	{

	}
	InputLayout() = default;
};
class InputLayoutManager
{
public:
	static InputLayout* g_pInputLayout;
	// ΩÃ±€≈Ê ∆–≈œ
	static InputLayoutManager& GetInstance()
	{
		static InputLayoutManager mgr;
		return mgr;
	}
	InputLayout* Load(ID3DBlob* pCode, std::wstring key);
	InputLayout* Load(ID3DBlob* pCode,
		D3D11_INPUT_ELEMENT_DESC layout[],
		UINT szNumCounter,
		std::wstring key);
	InputLayout* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Init(ID3DBlob* pCode);
private:
	std::map<std::wstring, InputLayout*> maplist;
	InputLayoutManager() {}
public:
	~InputLayoutManager();
};
#define I_InputLayout InputLayoutManager::GetInstance()