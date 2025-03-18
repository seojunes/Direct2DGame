#pragma once
#include "TStd.h"
class TDxState
{
public:
	static ComPtr<ID3D11SamplerState> m_pLinearSS;
	static ComPtr<ID3D11SamplerState> m_pPointSS;
	static ComPtr<ID3D11BlendState> m_pAlphaBlend;
	static ComPtr<ID3D11RasterizerState> m_pRSSolid;
	static ComPtr<ID3D11RasterizerState> m_pRSSolidNone;
	static ComPtr<ID3D11RasterizerState> m_pRSWireFrame;
	static ComPtr<ID3D11DepthStencilState> m_pDSSDepthEnable;
	static ComPtr<ID3D11DepthStencilState> m_pDSSDepthEnableZero;
	static ComPtr<ID3D11DepthStencilState> m_pDSSDepthDisableZero;
	static ComPtr<ID3D11DepthStencilState> m_pDSSDepthDisable;
	static void Create();
	static void Release();
};

