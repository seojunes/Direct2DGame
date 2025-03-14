#pragma once
#include "TStd.h"
class TDevice
{
public:
    // 장치 인터페이스 획득(생성)
    static ComPtr<ID3D11Device>         m_pd3dDevice;
    static ComPtr<ID3D11DeviceContext>  m_pd3dContext;
    static ComPtr<IDXGISwapChain>       m_pSwapChain;
    static ComPtr<ID3D11RenderTargetView> m_pRTV;
    static ComPtr<ID3D11DepthStencilView> m_pDSV;
    ComPtr<ID3D11Texture2D> m_pDepthStencilTexture = nullptr;
    static D3D11_VIEWPORT			    m_MainVP;
    static bool         m_bWireFrame;
    static bool         m_DepthEnable;
public:
    bool   CreateDevice();
    bool   CreateDepthStencilBuffer();
    void   Init();
    void   Frame();
    void   PreRender();
    void   Render();
    void   PostRender();
    void   Release();
};

