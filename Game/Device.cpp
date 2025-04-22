#include "Device.h"
#include "DxState.h"

ComPtr<ID3D11Device> Device::m_pd3dDevice = nullptr;  // ����
ComPtr<ID3D11DeviceContext>     Device::m_pd3dContext = nullptr; // �,����
ComPtr<IDXGISwapChain>          Device::m_pSwapChain = nullptr;
ComPtr<ID3D11RenderTargetView>  Device::m_pRTV = nullptr;
D3D11_VIEWPORT			Device::m_MainVP;
bool        Device::m_bWireFrame = false;

bool   Device::CreateDevice()
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
    // viewport
    m_MainVP.Width = (FLOAT)g_ptClientSize.x;
    m_MainVP.Height = (FLOAT)g_ptClientSize.y;
    m_MainVP.MinDepth = 0.0f;
    m_MainVP.MaxDepth = 1.0f;
    m_MainVP.TopLeftX = 0;
    m_MainVP.TopLeftY = 0;

    m_pd3dContext->RSSetViewports(1, &m_MainVP);
    m_pd3dContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), NULL);
    return true;
}
void   Device::Init()
{
    if (!CreateDevice())
    {

    }
}
void   Device::Frame()
{
    int a = 0;
}
void   Device::PreRender()
{
    float ClearColor[] = { 0.0f, 0.0f,0.0f, 1.0f };
    m_pd3dContext->ClearRenderTargetView(m_pRTV.Get(), ClearColor);
    m_pd3dContext->PSSetSamplers(0, 1, DxState::m_pPointSS.GetAddressOf());
    m_pd3dContext->OMSetBlendState(DxState::m_pAlphaBlend.Get(), 0, -1);
    if (!m_bWireFrame)
    {
        m_pd3dContext->RSSetState(DxState::m_pRSSolid.Get());
    }
    else
    {
        m_pd3dContext->RSSetState(DxState::m_pRSWireFrame.Get());
    }
}
void   Device::Render()
{
}
void   Device::PostRender()
{
    m_pSwapChain->Present(0, 0);
}
void   Device::Release()
{
}