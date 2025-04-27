#include "DxState.h"
#include "Device.h"
ComPtr<ID3D11SamplerState> DxState::m_pLinearSS = nullptr;
ComPtr<ID3D11SamplerState> DxState::m_pPointSS = nullptr;
ComPtr<ID3D11BlendState> DxState::m_pAlphaBlend = nullptr;
ComPtr<ID3D11RasterizerState> DxState::m_pRSSolid = nullptr;
ComPtr<ID3D11RasterizerState> DxState::m_pRSWireFrame = nullptr;
ComPtr<ID3D11Buffer> DxState::m_pCBScroll = nullptr;

void  DxState::Create()
{
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = TRUE;
	bd.RenderTarget[0].BlendEnable = TRUE;

	// rgb ȥ�տ���( ���ĺ��� ���� )
	// �÷�(rgb)��� = SRC * �ҽ����İ� + DEST * 1-�ҽ����İ�
	//  A = 0; A = 1  A=0.5
	// SRC�÷� : �Ƚ����̴��� �����
	// DEST�÷� : �̹� ��µǾ� �ִ� ����� �÷�
	// �÷�(rgb)��� = SRC * 1 + DEST * 0
	//     Final R = SRC.r * 1 + DEST.r * 0
	//     Final G = SRC.g * 1 + DEST.g * 0
	//     Final B = SRC.b * 1+ DEST.b * 0
	//        �ҽ��÷� * SrcBlend 
	//                  +             BlendOp
	//        ����÷� * DestBlend
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// A ���İ� ����
	//  Final A = SRC.a * 1 + DEST.a * 0
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = Device::m_pd3dDevice->CreateBlendState(&bd,
		m_pAlphaBlend.GetAddressOf());
	if (FAILED(hr))
	{

	}
	/// <summary>
	/// ���÷� ����
	/// </summary>
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = Device::m_pd3dDevice->CreateSamplerState(&sd,
		m_pLinearSS.GetAddressOf());
	if (FAILED(hr))
	{

	}
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = Device::m_pd3dDevice->CreateSamplerState(&sd,
		m_pPointSS.GetAddressOf());
	if (FAILED(hr))
	{

	}
	
	D3D11_BUFFER_DESC scrollDesc = {};
	scrollDesc.ByteWidth = sizeof(float) * 4; // float2 + padding
	scrollDesc.Usage = D3D11_USAGE_DEFAULT;
	scrollDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	scrollDesc.CPUAccessFlags = 0;

	hr = Device::m_pd3dDevice->CreateBuffer(&scrollDesc, nullptr, DxState::m_pCBScroll.GetAddressOf());
	if (FAILED(hr))
	{
		// ���� ó�� (�α� ��)
	}


	// �����Ͷ����� ����
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	hr = Device::m_pd3dDevice->CreateRasterizerState(
		&rsDesc, m_pRSSolid.GetAddressOf());
	if (FAILED(hr))
	{

	}
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = Device::m_pd3dDevice->CreateRasterizerState(
		&rsDesc, m_pRSWireFrame.GetAddressOf());
	if (FAILED(hr))
	{

	}
}

void  DxState::Release()
{
}