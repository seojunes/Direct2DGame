#include "TInputLayout.h"
#include "TDevice.h"
TInputLayout* TTInputLayoutManager::g_pInputLayout = nullptr;

bool		TInputLayout::Load(ID3DBlob* pCode, 
								D3D11_INPUT_ELEMENT_DESC layout[],
								UINT szNumCounter)
{	
	if (pCode == nullptr)
	{
		return true;
	}
	HRESULT hr = TDevice::m_pd3dDevice->CreateInputLayout(
		layout,
		szNumCounter,
		pCode->GetBufferPointer(),
		pCode->GetBufferSize(),
		m_pInputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		DX_CHECK(hr, _T(__FUNCTION__));
		return false;
	}
	return true;
}
ID3D11InputLayout* TInputLayout::Get()
{
	return m_pInputLayout.Get();
}
void TInputLayout::Release()
{	
	m_pInputLayout = nullptr;
}
void     TTInputLayoutManager::Init(ID3DBlob* pCode)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// 0~8
		{ "POS",  0, DXGI_FORMAT_R32G32_FLOAT,		 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT  iNumCnt = sizeof(layout) / sizeof(layout[0]);
	g_pInputLayout = Load(pCode, layout, iNumCnt, L"PCT");
}
TInputLayout* TTInputLayoutManager::Load(ID3DBlob* pCode, 
									D3D11_INPUT_ELEMENT_DESC layout[],
									UINT szNumCounter,
									std::wstring key)
{
	auto data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}
	TInputLayout* pData = new TInputLayout(key);
	if (pData->Load(pCode, layout, szNumCounter))
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
std::wstring TTInputLayoutManager::SplitPath(std::wstring file)
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
TInputLayout* TTInputLayoutManager::GetPtr(std::wstring key)
{
	auto data = maplist.find(key);
	if (data != maplist.end())
	{
		return data->second;
	}
	return nullptr;
}

TTInputLayoutManager::~TTInputLayoutManager()
{
	for (auto sound : maplist)
	{
		sound.second->Release();
		delete sound.second;
	}
	maplist.clear();
}