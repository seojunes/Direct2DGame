#include "TTexture.h"
#include "TDevice.h"
bool		TTexture::Load(std::wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		TDevice::m_pd3dDevice.Get(),
		filename.c_str(),
		(ID3D11Resource**)&m_pTexture,
		&m_pTexSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			TDevice::m_pd3dDevice.Get(),
			filename.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pTexSRV);
		if (FAILED(hr))
		{
			//tga format    directx tk -> directx tex 
			//DX_CHECK(hr, _T(__FUNCTION__));
			return false;
		}		
	}	
	m_pTexture->GetDesc(&m_TexDesc);
	return true;
}
void TTexture::Release()
{
	if(m_pTexture) m_pTexture->Release();
	if (m_pTexSRV) m_pTexSRV->Release();
	m_pTexture = nullptr;
	m_pTexSRV = nullptr;
}
TTexture* TTextureManager::Load(std::wstring filename)
{
	auto key = SplitPath(filename);
	auto data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}

	TTexture* pData = new TTexture(key);

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
std::wstring TTextureManager::SplitPath(std::wstring file)
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
TTexture* TTextureManager::GetPtr(std::wstring key)
{
	auto data = maplist.find(key);
	if (data != maplist.end())
	{
		return data->second;
	}
	return nullptr;
}

TTextureManager::~TTextureManager()
{
	for (auto sound : maplist)
	{
		sound.second->Release();
		delete sound.second;
	}
	maplist.clear();
}