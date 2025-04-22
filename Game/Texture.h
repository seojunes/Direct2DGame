#pragma once
#include "Std.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
class Texture
{
public:
	std::wstring   m_csName;
	D3D11_TEXTURE2D_DESC m_TexDesc;
	ID3D11Texture2D* m_pTexture = nullptr;
	ID3D11ShaderResourceView* m_pTexSRV = nullptr;
	virtual  bool  Load(std::wstring filename);
	virtual  void  Release();
	Texture(std::wstring key) : m_csName(key) {}
	Texture() = default;
};

class TextureManager
{
public:
	static TextureManager& GetInstance()
	{
		static TextureManager mgr;
		return mgr;
	}
	Texture* Load(std::wstring filename);
	Texture* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Frame();
	void		Render();
private:
	std::map<std::wstring, Texture*> maplist;
	TextureManager() {}
public:
	~TextureManager();
};

#define I_Tex TextureManager::GetInstance()