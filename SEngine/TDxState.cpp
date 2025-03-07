#include "TDxState.h"
#include "TDevice.h"
ComPtr<ID3D11SamplerState> TDxState::m_pLinearSS = nullptr;
ComPtr<ID3D11SamplerState> TDxState::m_pPointSS = nullptr;
ComPtr<ID3D11BlendState> TDxState::m_pAlphaBlend = nullptr;
ComPtr<ID3D11RasterizerState> TDxState::m_pRSSolid = nullptr;
ComPtr<ID3D11RasterizerState> TDxState::m_pRSWireFrame = nullptr;

void  TDxState::Create()
{
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = TRUE;
	bd.RenderTarget[0].BlendEnable  =TRUE;

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
	bd.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = 
		D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = TDevice::m_pd3dDevice->CreateBlendState(&bd,
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
    sd.AddressU= D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV= D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW= D3D11_TEXTURE_ADDRESS_WRAP;    
	 hr = TDevice::m_pd3dDevice->CreateSamplerState(&sd, 
		 m_pLinearSS.GetAddressOf());
	if (FAILED(hr))
	{

	}
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = TDevice::m_pd3dDevice->CreateSamplerState(&sd, 
		m_pPointSS.GetAddressOf());
	if (FAILED(hr))
	{

	}

	// �����Ͷ����� ����
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	hr = TDevice::m_pd3dDevice->CreateRasterizerState(
		&rsDesc, m_pRSSolid.GetAddressOf());
	if (FAILED(hr))
	{

	}
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = TDevice::m_pd3dDevice->CreateRasterizerState(
		&rsDesc, m_pRSWireFrame.GetAddressOf());
	if (FAILED(hr))
	{

	}
}

void  TDxState::Release()
{
}