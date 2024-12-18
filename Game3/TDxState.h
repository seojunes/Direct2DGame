#pragma once
#include "TStd.h"
class TDxState
{
public:
	static ComPtr<ID3D11SamplerState> m_pLinearSS;
	static ComPtr<ID3D11SamplerState> m_pPointSS;
	static ComPtr<ID3D11BlendState> m_pAlphaBlend;
	static void Create();
	static void Release();
};

