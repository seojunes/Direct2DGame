#include "SEngine.h"
#include "TDxState.h"
TCamera* SEngine::g_pCamera = nullptr;
void   SEngine::CoreInit()
{
    m_DxDevice.Init();
    m_GameTimer.Init();
    m_Input.Init();
    m_DxWrite.Init();
    // 3d(RT) : 2d(RT) ¿¬µ¿
    IDXGISurface* pBackBuffer;
    HRESULT hr = m_DxDevice.m_pSwapChain->GetBuffer(
        0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
    if (SUCCEEDED(m_DxWrite.Create(pBackBuffer)))
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
    g_pCamera = m_pSceneCamera.get();

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
    
    float fYaw = 0;
    float fPitch = 0;
    fYaw = -g_ptDeltaMouse.x * g_fSPF;
    fPitch = -g_ptDeltaMouse.y * g_fSPF;

    float fDistance = 0.0f;
    if (g_GameKey.dw7key == KEY_HOLD)
    {
        g_pCamera->m_vPosition -= g_pCamera->m_vLook * g_fSPF * 20.0f;
    }
    if (g_GameKey.dw9key == KEY_HOLD)
    {
        g_pCamera->m_vPosition += g_pCamera->m_vLook * g_fSPF * 20.0f ;
    }
    if (g_GameKey.dw4key == KEY_HOLD)
    {
        g_pCamera->m_vPosition += g_pCamera->m_vRight * g_fSPF * 20.0f;
    }
    if (g_GameKey.dw6key == KEY_HOLD)
    {
        g_pCamera->m_vPosition -= g_pCamera->m_vRight * g_fSPF * 20.0f;
    }
    if (g_GameKey.dw8key == KEY_HOLD)
    {
        g_pCamera->m_vPosition += g_pCamera->m_vUp * g_fSPF * 20.0f;
    }
    if (g_GameKey.dw5key == KEY_HOLD)
    {
        g_pCamera->m_vPosition -= g_pCamera->m_vUp * g_fSPF * 20.0f;
    }
    if (g_GameKey.dw0key == KEY_PUSH)
    {
        g_pCamera->m_vPosition = { 0.0f,0.0f,-10.0f };
        m_pSceneCamera->Update(TVector4(0, 0, 0, 0), true);
    }
    
    
    if (m_nMouseWheelDelta != 0)
    {
        fDistance = ((m_nMouseWheelDelta) > 0) ? (1.0f) : (-1.0f);
        fDistance = fDistance * g_fSPF * 300.0f;
    }
    m_pSceneCamera->Update(TVector4(fPitch, fYaw, 0, fDistance));

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
                m_DxWrite.Draw(rt, m_Input.m_szTime, 
                                    D2D1::ColorF(1, 0, 0,1));
            
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