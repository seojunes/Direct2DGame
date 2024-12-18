#pragma once
#include "TStd.h"
class TShader
{
public:
	std::wstring   m_csName;
	ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	ComPtr<ID3DBlob> m_pCode;
	ComPtr<ID3D11PixelShader>  m_pPixelShader = nullptr;
	virtual  bool  Load(std::wstring filename);
	virtual  bool  LoadVertexShader(std::wstring filename);
	virtual  bool  LoadPixelShader(std::wstring filename);
	
	virtual  void  Release();
	TShader(std::wstring key) : m_csName(key)
	{

	}
	TShader() = default;
};
class TVShader : public TShader
{

};
class TPShader : public TShader
{

};
class TShaderManager
{
public:
	static TShader*				g_pDefaultShader;
	// ΩÃ±€≈Ê ∆–≈œ
	static TShaderManager& GetInstance()
	{
		static TShaderManager mgr;
		return mgr;
	}
	TShader* Load(std::wstring filename);
	TShader* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Init();
	void		Frame();
	void		Render();
private:
	std::map<std::wstring, TShader*> maplist;
	TShaderManager() {}
public:
	~TShaderManager();
};

#define I_Shader TShaderManager::GetInstance()