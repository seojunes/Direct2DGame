#include "TShader.h"
#include "TDevice.h"
TShader* TShaderManager::g_pDefaultShader = nullptr;

bool		TShader::Load(std::wstring filename)
{
	if (!LoadVertexShader(filename))
	{
		return false;
	}
	if (!LoadPixelShader(filename))
	{
		return false;
	}
	return true;
}
bool		TShader::LoadPixelShader(std::wstring filename)
{
	// hlsl  컴파일
	ComPtr<ID3DBlob> pErrorCode = nullptr;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	// hlsl  컴파일
	ComPtr<ID3DBlob> pCode = nullptr;
	HRESULT hr = D3DCompileFromFile(filename.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0", flags, 0,
		pCode.GetAddressOf(),
		pErrorCode.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBoxA(g_hWnd,
			(char*)pErrorCode->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}
	hr = TDevice::m_pd3dDevice->CreatePixelShader(
		pCode->GetBufferPointer(),
		pCode->GetBufferSize(),
		nullptr,
		m_pPixelShader.GetAddressOf());
	if (FAILED(hr))
	{
		DX_CHECK(hr, _T(__FUNCTION__));
		return false;
	}	
	return true;
}
bool		TShader::LoadVertexShader(std::wstring filename)
{
	// hlsl  컴파일
	ComPtr<ID3DBlob> pErrorCode = nullptr;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	HRESULT hr = D3DCompileFromFile(filename.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0", flags, 0,
		m_pCode.GetAddressOf(),
		pErrorCode.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBoxA(g_hWnd,
			(char*)pErrorCode->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}
	hr = TDevice::m_pd3dDevice->CreateVertexShader(
		m_pCode->GetBufferPointer(),
		m_pCode->GetBufferSize(),
		nullptr,
		m_pVertexShader.GetAddressOf());
	if (FAILED(hr))
	{
		DX_CHECK(hr, _T(__FUNCTION__));
		return false;
	}	
	return true;
}
void TShader::Release()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pCode = nullptr;
}
void     TShaderManager::Init()
{
	g_pDefaultShader = Load(
		L"../../data/shader/pnct.txt");	
}
TShader* TShaderManager::Load(std::wstring filename)
{
	auto key = SplitPath(filename);
	auto data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}
	
	TShader* pData = new TShader(key);

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
std::wstring TShaderManager::SplitPath(std::wstring file)
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
TShader* TShaderManager::GetPtr(std::wstring key)
{
	auto data = maplist.find(key);
	if (data != maplist.end())
	{
		return data->second;
	}
	return nullptr;
}

TShaderManager::~TShaderManager()
{
	for (auto sound : maplist)
	{
		sound.second->Release();
		delete sound.second;
	}	
	maplist.clear();
}