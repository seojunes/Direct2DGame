#pragma once
#include "TStd.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
class TTexture
{
public:
	std::wstring   m_csName;
	D3D11_TEXTURE2D_DESC m_TexDesc;
	ID3D11Texture2D* m_pTexture = nullptr;
	ID3D11ShaderResourceView* m_pTexSRV = nullptr;
	virtual  bool  Load(std::wstring filename);
	virtual  void  Release();
	TTexture(std::wstring key) : m_csName(key)
	{

	}
	TTexture() = default;
};

class TTextureManager
{	
public:

	// ΩÃ±€≈Ê ∆–≈œ
	static TTextureManager& GetInstance()
	{
		static TTextureManager mgr;
		return mgr;
	}
	TTexture* Load(std::wstring filename);
	TTexture* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Frame();
	void		Render();
private:
	std::map<std::wstring, TTexture*> maplist;
	TTextureManager() {}
public:
	~TTextureManager();
};

#define I_Tex TTextureManager::GetInstance()