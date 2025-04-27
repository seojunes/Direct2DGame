#pragma once
#include "Std.h"
class DxState
{
public:
	static ComPtr<ID3D11SamplerState> m_pLinearSS;
	static ComPtr<ID3D11SamplerState> m_pPointSS;
	static ComPtr<ID3D11BlendState> m_pAlphaBlend;
	static ComPtr<ID3D11RasterizerState> m_pRSSolid;
	static ComPtr<ID3D11RasterizerState> m_pRSWireFrame;
	static ComPtr<ID3D11Buffer> m_pCBScroll; // 바람효과 시간용 상수 버퍼 추가
	static void Create();
	static void Release();
};

