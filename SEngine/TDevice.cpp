#include "TDevice.h"
#include "TDxState.h"

ComPtr<ID3D11Device> TDevice::m_pd3dDevice = nullptr;  // ����
ComPtr<ID3D11DeviceContext>     TDevice::m_pd3dContext = nullptr; // �,����
ComPtr<IDXGISwapChain>          TDevice::m_pSwapChain = nullptr;
ComPtr<ID3D11RenderTargetView>  TDevice::m_pRTV = nullptr;
ComPtr<ID3D11DepthStencilView>  TDevice::m_pDSV = nullptr;
D3D11_VIEWPORT			        TDevice::m_MainVP;
bool                            TDevice::m_bWireFrame = false;
bool                            TDevice::m_DepthEnable = true;
bool   TDevice::CreateDepthStencilBuffer()
{
    HRESULT hr;
    //1) ���� / ���ٽ� ������ �ؽ�ó ����
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = g_ptClientSize.x;
    descDepth.Height = g_ptClientSize.y;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = m_pd3dDevice->CreateTexture2D(&descDepth, NULL,
        m_pDepthStencilTexture.GetAddressOf());
    if (FAILED(hr))
    {
        DX_CHECK(hr, _T("CreateTexture2D"));
        return false;
    }
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = descDepth.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    if (FAILED(hr = m_pd3dDevice->CreateDepthStencilView(
        m_pDepthStencilTexture.Get(), &dsvDesc,
        m_pDSV.GetAddressOf())))
    {
        DX_CHECK(hr, _T("CreateDepthStencilView"));
        return false;
    }
    return true;
}
bool   TDevice::CreateDevice()
{
    D3D_FEATURE_LEVEL pFeatureLevel;
    //  �� ���÷��� �׷���ī��
    IDXGIAdapter* pAdapter = nullptr;
    // �׷��� ī���� �ϵ���� ����(�ִ뼺��)
    D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;
    // ȭ�� �׷��Ƚ��� ����� ��Ÿ ���̺귯���� ����ϰڴ�. 
    HMODULE Software = NULL;
    UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // dwrite���� �ʼ�
#ifdef _DEBUG
    Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
    UINT FeatureLevels = 1;
    UINT SDKVersion = D3D11_SDK_VERSION;// ��ġ�Ǿ� �ִ� ������ ����ϰڴ�.

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    // ������۸�( ����Ʈ ����, ����� <-> ȭ�����) 
    // �����
    scd.BufferCount = 1;
    scd.BufferDesc.Width = g_ptClientSize.x;
    scd.BufferDesc.Height = g_ptClientSize.y;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = g_hWnd;
    scd.Windowed = true;
    // �ֻ���
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    // ���ø�.
    scd.SampleDesc.Count = 1;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        pAdapter,
        DriverType,
        Software,
        Flags,
        &pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        &scd,
        // ��ȯ��
        m_pSwapChain.GetAddressOf(),
        m_pd3dDevice.GetAddressOf(),
        &pFeatureLevel,//D3D_FEATURE_LEVEL_11_0
        m_pd3dContext.GetAddressOf());

    if (FAILED(hr))
    {
        DX_CHECK(hr, _T(__FUNCTION__));
        return false;
    }

    //RTV ������� �����ּҸ� ��´�.
    ID3D11Texture2D* pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        DX_CHECK(hr, _T(__FUNCTION__));
        return false;
    }
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
        m_pRTV.GetAddressOf());
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        DX_CHECK(hr, _T(__FUNCTION__));
        return false;
    }
    /// ���� /���ٽ� ���� ����
    CreateDepthStencilBuffer();

    // viewport
    m_MainVP.Width = (FLOAT)g_ptClientSize.x;
    m_MainVP.Height = (FLOAT)g_ptClientSize.y;
    m_MainVP.MinDepth = 0.0f;
    m_MainVP.MaxDepth = 1.0f;
    m_MainVP.TopLeftX = 0;
    m_MainVP.TopLeftY = 0;


    return true;
}
void   TDevice::Init()
{
    if (!CreateDevice())
    {

    }
}
void   TDevice::Frame()
{
    int a = 0;
}
void   TDevice::PreRender()
{
    m_pd3dContext->RSSetViewports(1, &m_MainVP);
    m_pd3dContext->OMSetRenderTargets(1,
        m_pRTV.GetAddressOf(),
        m_pDSV.Get());

    float ClearColor[] = { 0.0f, 0.0f,0.0f, 1.0f };
    m_pd3dContext->ClearRenderTargetView(m_pRTV.Get(), ClearColor);
    m_pd3dContext->ClearDepthStencilView(m_pDSV.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
}
void TDevice::SetDefaultState()
{
    m_pd3dContext->PSSetSamplers(0, 1, TDxState::m_pLinearSS.GetAddressOf());
    m_pd3dContext->OMSetBlendState(TDxState::m_pAlphaBlend.Get(), 0, -1);
    {// ���̾������� ����
        if (!m_bWireFrame)
        {
            m_pd3dContext->RSSetState(TDxState::m_pRSSolid.Get());
        }
        else
        {
            m_pd3dContext->RSSetState(TDxState::m_pRSWireFrame.Get());
        }
    }
    
    {// Depth(Z����) ����
        if (m_DepthEnable)
        {
            m_pd3dContext->OMSetDepthStencilState(TDxState::m_pDSSDepthEnable.Get(), 0);
        }
        else
        {
            m_pd3dContext->OMSetDepthStencilState(TDxState::m_pDSSDepthDisable.Get(), 0);
        }
    }
}
void   TDevice::Render()
{
}
void   TDevice::PostRender()
{
    m_pSwapChain->Present(0, 0);
}
void   TDevice::Release()
{
}