#include "SEngine.h"
#include "TDxState.h"
TCamera* SEngine::g_pCamera = nullptr;
void SEngine::SetCamera(TCamera* pCamera)
{
    if (pCamera == nullptr)
    {
        g_pCamera = m_pSceneCamera.get();
        return;
    }
    g_pCamera = pCamera;
}
void   SEngine::CoreInit()
{
    m_DxDevice.Init();
    m_GameTimer.Init();
    m_Input.Init();
    m_DxWrite.Init();
    // 3d(RT) : 2d(RT) ¿¬µ¿
    IDXGISurface* pBackBuffer;
    HRESULT hr = m_DxDevice.m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
    if ( SUCCEEDED(m_DxWrite.Create(pBackBuffer)))
    {
        pBackBuffer->Release();
    }

    TDxState::Create();

    I_Shader.Init();
    if (I_Shader.g_pDefaultShader != nullptr)
    {
        I_InputLayout.Init(I_Shader.g_pDefaultShader->m_pCode.Get());
    }

    m_pSceneCamera = std::make_shared<TCamera>();
    SetCamera(m_pSceneCamera.get());

    TVector3 vCameraPos = TVector3(0, 0, -10.0f);
    TVector3 vCameraTarget = TVector3(0, 0, 0.0f);
    TVector3 vCameraUp = TVector3(0, 1.0f, 0.0f);
    m_pSceneCamera->CreateViewMatrix(vCameraPos, vCameraTarget, vCameraUp);

    float fAspect = (float)g_ptClientSize.x / (float)g_ptClientSize.y;
    float fFovY = (float)T_Pi * 0.25f;
    float fNearZ = 1.0f;
    float fFarZ = 1000.0f;
    m_pSceneCamera->CreateProjMatrix(fFovY, fAspect, fNearZ, fFarZ);

    Init();
}
void   SEngine::CoreFrame() 
{       
    m_GameTimer.Frame();
    m_Input.Frame();
    m_DxWrite.Frame();
    m_DxDevice.Frame();

    if (m_Input.KeyCheck('V') == KEY_PUSH)
    {
        TDevice::m_bWireFrame = !TDevice::m_bWireFrame;
    }
    if (m_Input.KeyCheck('B') == KEY_PUSH)
    {
        TDevice::m_DepthEnable = !TDevice::m_DepthEnable;
    }

    g_pCamera->Tick();
    
    
    Tick();
}
void   SEngine::CoreRender() 
{
    m_DxDevice.PreRender();  
    {
        Render();        

        m_DxWrite.PreRender();
            {
                D2D1_RECT_F rt = { 0.0f, 0.0f, 800.0f, 600.0f };
                m_DxWrite.Draw(rt, m_GameTimer.m_szTime);
                rt.top += 25.0f;
                m_DxWrite.Draw(rt, m_Input.m_szTime, D2D1::ColorF(1, 0, 0,1));
            }
            m_DxWrite.Render();
        m_DxWrite.PostRender();
    }
    m_DxDevice.PostRender();

    if (g_GameKey.dwExit == KEY_PUSH)
    {
        m_bRun = false;
    }
}
void   SEngine::CoreRelease()
{
    m_GameTimer.Release();
    m_DxDevice.Release();
    m_Input.Release();
    m_DxWrite.Release();
    TDxState::Release();

    Destroy();
}
bool SEngine::GameRun()
{
    CoreInit();
    while (m_bRun)
    {        
        if(!MessageProcess())
        {
            CoreFrame();
            CoreRender();       
            m_nMouseWheelDelta = 0;
        }  
    }	
    CoreRelease();
	return true;
}