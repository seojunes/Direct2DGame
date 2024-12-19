#include "TObject.h"
#include "TDevice.h"

// 속성->C/C++->일반->추가 포함 디렉토리(./Inc)
TObject& TObject::SetShader(TShader* pData)
{
	if (pData == nullptr)
	{
		m_pShader = I_Shader.g_pDefaultShader;
	}
	else
	{
		m_pShader = pData;
	}
	return *this;
}
TObject& TObject::SetTexture(TTexture* pData)
{
	m_pTexture = pData;
	return *this;
}
TObject& TObject::SetLayout(TInputLayout* pData)
{
	if (pData == nullptr)
	{
		m_pInputLayout = I_InputLayout.g_pInputLayout;
	}
	else
	{
		m_pInputLayout = pData;
	}
	return *this;
}
bool   TObject::LoadTexrture(std::wstring texName)
{
	m_pTexture = I_Tex.Load(texName);	
	if (m_pTexture==nullptr)
	{		
		return false;
	}

	return true;
}
void	TObject::Init()
{}
void	TObject::Frame()
{}
void	TObject::PreRender()
{
	TDevice::m_pd3dContext->PSSetShaderResources(
		0, 1, &m_pTexture->m_pTexSRV);
	TDevice::m_pd3dContext->VSSetShader(
		m_pShader->m_pVertexShader.Get(), nullptr, 0);
	TDevice::m_pd3dContext->PSSetShader(
		m_pShader->m_pPixelShader.Get(), nullptr, 0);
	TDevice::m_pd3dContext->IASetInputLayout(
		m_pInputLayout->Get());

	// 정점버퍼에서 Offsets에서 시작하여
	// Strides크기로 정점을 정점쉐이더로 전달해라.
	UINT Strides = sizeof(PCT_VERTEX);
	UINT Offsets = 0;
	TDevice::m_pd3dContext->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&Strides,
		&Offsets);
	TDevice::m_pd3dContext->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, 0);
	TDevice::m_pd3dContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void	TObject::Render()
{	
	PreRender();
	PostRender();
}
void	TObject::PostRender()
{
	if (m_pIndexBuffer == nullptr)
		TDevice::m_pd3dContext->Draw(m_vVertexList.size(), 0);
	else
		TDevice::m_pd3dContext->DrawIndexed(m_vIndexList.size(), 0, 0);
}
void	TObject::Release()
{
	
}
bool	TObject::Create()
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
bool	TObject::Create(TLoadResData data)
{
	m_LoadResData = data;
	if (!LoadTexrture(m_LoadResData.texPathName))
	{
		return false;
	}
	return Create();
}
bool	TObject::Create(TLoadResData data,
						TVertex2 s,
						TVertex2 t)
{
	m_LoadResData = data;
	m_srtScreen.SetP( s, t );
	if (!LoadTexrture(m_LoadResData.texPathName))
	{
		return false;
	}
	return Create();
}
void    TObject::SetVertexData()
{
	// 정규화장치(NDC)좌표계(x,y)	
	// v0:-1,1     0,1          v1:1, 1
	//    -1,0     0,0          1, 0
	// v2:1,-1     0,-1         v3:1,-1
	// 화면좌표계  <-> 변환  <-> 직각좌표계
	// 직각좌표계  <-> 변환  <-> NDC좌표계
	// NDC좌표계  <-> 변환  <-> 직각좌표계
	m_vVertexList.resize(4);
	TVertex2 s = { m_srtScreen.x, m_srtScreen.y };
	// NDC <- Screen
	s.x = m_srtScreen.x / g_ptClientSize.x; // 0 ~1
	s.y = m_srtScreen.y / g_ptClientSize.y; // 0 ~1
	s.x = s.x * 2.0f - 1.0f;
	s.y = -(s.y * 2.0f - 1.0f);
	TVertex2 t;
	t.x = (m_srtScreen.x + m_srtScreen.w) / g_ptClientSize.x;
	t.y = (m_srtScreen.y + m_srtScreen.h) / g_ptClientSize.y;
	t.x = t.x * 2.0f - 1.0f;
	t.y = (t.y * 2.0f - 1.0f)*-1.0f;
	m_vVertexList[0].v = s;
	m_vVertexList[1].v = { t.x, s.y };
	m_vVertexList[2].v = { s.x, t.y };
	m_vVertexList[3].v = t; 

	m_vVertexList[0].c = { 1.0f,0.0f,0.0f,1.0f};		
	m_vVertexList[1].c = { 0.0f,1.0f,0.0f,1.0f };
	m_vVertexList[2].c = { 0.0f,0.0f,1.0f,1.0f };	
	m_vVertexList[3].c = { 1.0f,1.0f,1.0f,1.0f };

	m_vVertexList[0].t = { 0.0f,0.0f };	
	m_vVertexList[1].t = { 1.0f,0.0f };
	m_vVertexList[2].t = { 0.0f,1.0f };
	m_vVertexList[3].t = { 1.0f,1.0f };
}
bool	TObject::CreateVertexBuffer() 
{
	// 화면좌표계(x,y)
	// v1:0,0      ->      v2:800, 0
	//   |	     400,300
	// v3:0,600            v3:800,600
	// 
	// 직각좌표계(x,y)	
	// v1:-400,300    ->       v2:400, 400
	//              0,0
	// v3:0,-300               v3:400,-300

	// 화면좌표계  <-> 변환  <-> 직각좌표계
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PCT_VERTEX)* m_vVertexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vVertexList.at(0);
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd,	m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void    TObject::SetIndexData()
{
	m_vIndexList.resize(6);
	m_vIndexList[0] = 0;	m_vIndexList[1] = 1;
	m_vIndexList[2] = 2;
	m_vIndexList[3] = 2;
	m_vIndexList[4] = 1;
	m_vIndexList[5] = 3;
}
bool	TObject::CreateIndexBuffer()
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
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pIndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	TObject::CreateVertexShader() 
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
bool	TObject::CreatePixelShader() 
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
bool	TObject::CreateInputLayout() 
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
	m_pInputLayout = I_InputLayout.Load(m_pShader->m_pCode.Get(),layout, 3, L"PCT");
	return true;
}
TObject::TObject()
{
	m_srtScreen.SetS(0.0f, 0.0f, (float)g_ptClientSize.x, (float)g_ptClientSize.y);
}
TObject::~TObject()
{
}
