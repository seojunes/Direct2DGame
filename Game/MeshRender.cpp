#include "MeshRender.h"
#include "Device.h"

// 속성->C/C++->일반->추가 포함 디렉토리(./Inc)
bool MeshRender::SetShader(Shader* pData)
{
	if (pData == nullptr)
	{
		m_pShader = I_Shader.g_pDefaultShader;
	}
	else
	{
		m_pShader = pData;
	}
	return true;
}
bool MeshRender::SetTexture(Texture* pData)
{
	m_pTexture = pData;
	return true;
}
bool MeshRender::SetLayout(InputLayout* pData)
{
	if (pData == nullptr)
	{
		m_pInputLayout = I_InputLayout.g_pInputLayout;
	}
	else
	{
		m_pInputLayout = pData;
	}
	return true;
}
bool   MeshRender::LoadTexture(std::wstring texName)
{
	m_pTexture = I_Tex.Load(texName);
	if (m_pTexture == nullptr)
	{
		return false;
	}

	return true;
}
void	MeshRender::Transform(TVector2 vCamera)
{
}
void	MeshRender::PreRender()
{
	Device::m_pd3dContext->PSSetShaderResources(
		0, 1, &m_pTexture->m_pTexSRV);
	Device::m_pd3dContext->VSSetShader(
		m_pShader->m_pVertexShader.Get(), nullptr, 0);
	Device::m_pd3dContext->PSSetShader(
		m_pShader->m_pPixelShader.Get(), nullptr, 0);
	Device::m_pd3dContext->IASetInputLayout(
		m_pInputLayout->Get());

	// 정점버퍼에서 Offsets에서 시작하여
	// Strides크기로 정점을 정점쉐이더로 전달해라.
	UINT Strides = sizeof(PCT_VERTEX);
	UINT Offsets = 0;
	Device::m_pd3dContext->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&Strides,
		&Offsets);
	Device::m_pd3dContext->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, 0);
	Device::m_pd3dContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void	MeshRender::Render()
{
	PreRender();

	PostRender();
}
void	MeshRender::PostRender()
{
	if (m_pIndexBuffer == nullptr)
		Device::m_pd3dContext->Draw(m_vVertexList.size(), 0);
	else
		Device::m_pd3dContext->DrawIndexed(m_vIndexList.size(), 0, 0);
}
void	MeshRender::Release()
{

}
bool	MeshRender::Create()
{
	SetVertexData();
	if (!CreateVertexBuffer())
	{
		return false;
	}
	SetIndexData();
	if (!CreateIndexBuffer())
	{
		return false;
	}
	if (!CreateVertexShader())
	{
		return false;
	}
	if (!CreatePixelShader())
	{
		return false;
	}
	if (!CreateInputLayout())
	{
		return false;
	}
	return true;
}
bool	MeshRender::Create(TLoadResData data)
{
	m_LoadResData = data;
	if (!LoadTexture(m_LoadResData.texPathName))
	{
		return false;
	}
	return Create();
}
void    MeshRender::SetVertexData()
{
	m_vScreenList.resize(4);
	m_vScreenList[0] = { -1.0f, -1.0f };
	m_vScreenList[1] = { +1.0f, -1.0f };
	m_vScreenList[2] = { -1.0f, +1.0f };
	m_vScreenList[3] = { +1.0f, +1.0f };
	m_vVertexList.resize(4);
	TVector2 s = m_vScreenList[0];
	TVector2 t = m_vScreenList[3];
	m_vVertexList[0].v = s;
	m_vVertexList[1].v = { t.x, s.y };
	m_vVertexList[2].v = { s.x, t.y };
	m_vVertexList[3].v = t;

	m_vVertexList[0].c = { 1.0f,1.0f,1.0f,1.0f };
	m_vVertexList[1].c = { 1.0f,1.0f,1.0f,1.0f };
	m_vVertexList[2].c = { 1.0f,1.0f,1.0f,1.0f };
	m_vVertexList[3].c = { 1.0f,1.0f,1.0f,1.0f };

	m_vVertexList[0].t = { 0.0f,0.0f };
	m_vVertexList[1].t = { 1.0f,0.0f };
	m_vVertexList[2].t = { 0.0f,1.0f };
	m_vVertexList[3].t = { 1.0f,1.0f };
}
bool	MeshRender::CreateVertexBuffer()
{
	// 화면좌표계  <-> 변환  <-> 직각좌표계
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PCT_VERTEX) * m_vVertexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vVertexList.at(0);
	HRESULT hr = Device::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void    MeshRender::SetIndexData()
{
	m_vIndexList.resize(6);
	m_vIndexList[0] = 0;
	m_vIndexList[1] = 1;
	m_vIndexList[2] = 2;
	m_vIndexList[3] = 2;
	m_vIndexList[4] = 1;
	m_vIndexList[5] = 3;
}
bool	MeshRender::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(DWORD) * m_vIndexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vIndexList.at(0);
	HRESULT hr = Device::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pIndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	MeshRender::CreateVertexShader()
{
	if (m_LoadResData.texShaderName.empty())
	{
		return true;
	}
	m_pShader = I_Shader.Load(m_LoadResData.texShaderName);
	if (m_pShader == nullptr)
	{
		return false;
	}
	return true;
}
bool	MeshRender::CreatePixelShader()
{
	if (m_LoadResData.texShaderName.empty())
	{
		return true;
	}
	if (m_pShader && m_pShader->m_pPixelShader == nullptr)
	{
		/*m_pShader = I_Shader.Load(m_LoadResData.texShaderName);
		if (m_pShader == nullptr)
		{
			return false;
		}*/
		return false;
	}
	return true;
}
bool	MeshRender::CreateInputLayout()
{
	if (m_pShader == nullptr) return true;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// 0~8
		{ "POS",  0, DXGI_FORMAT_R32G32_FLOAT,		 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT  iNumCnt = sizeof(layout) / sizeof(layout[0]);
	m_pInputLayout = I_InputLayout.Load(m_pShader->m_pCode.Get(), layout, 3, L"PCT");
	return true;
}
MeshRender::MeshRender()
{

}
MeshRender::~MeshRender()
{
}
