#pragma once
#include "Std.h"
class Shader
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
	Shader(std::wstring key) : m_csName(key)
	{

	}
	Shader() = default;
};

class ShaderManager
{
public:
	static Shader* g_pDefaultShader;
	// ΩÃ±€≈Ê ∆–≈œ
	static ShaderManager& GetInstance()
	{
		static ShaderManager mgr;
		return mgr;
	}
	Shader* Load(std::wstring filename);
	Shader* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Init();
	void		Frame();
	void		Render();
private:
	std::map<std::wstring, Shader*> maplist;
	ShaderManager() {}
public:
	~ShaderManager();
};

#define I_Shader ShaderManager::GetInstance()