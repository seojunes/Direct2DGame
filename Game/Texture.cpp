#include "Texture.h"
#include "Device.h"
bool		Texture::Load(std::wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		Device::m_pd3dDevice.Get(),
		filename.c_str(),
		(ID3D11Resource**)&m_pTexture,
		&m_pTexSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			Device::m_pd3dDevice.Get(),
			filename.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pTexSRV);
		if (FAILED(hr))
		{
			//tga format    directx tk -> directx tex 
			DX_CHECK(hr, _T(__FUNCTION__));
			return false;
		}
	}
	m_pTexture->GetDesc(&m_TexDesc);
	return true;
}
void Texture::Release()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pTexSRV) m_pTexSRV->Release();
	m_pTexture = nullptr;
	m_pTexSRV = nullptr;
}
Texture* TextureManager::Load(std::wstring filename)
{
	auto key = SplitPath(filename);
	auto data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}

	Texture* pData = new Texture(key);

	if (pData->Load(filename))
	{
		maplist.insert(std::make_pair(key, pData));
	}
	else
	{
		delete pData;
		pData = nullptr;
	}
	return pData;
}
std::wstring TextureManager::SplitPath(std::wstring file)
{
	TCHAR szFileName[256];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath_s(file.c_str(), Drive, Dir, FName, Ext);

	std::wstring key = FName;
	key += Ext;
	return key;
}
Texture* TextureManager::GetPtr(std::wstring key)
{
	auto data = maplist.find(key);
	if (data != maplist.end())
	{
		return data->second;
	}
	return nullptr;
}

TextureManager::~TextureManager()
{
	for (auto sound : maplist)
	{
		sound.second->Release();
		delete sound.second;
	}
	maplist.clear();
}