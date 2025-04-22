#include "GameCore.h"
#include "DxState.h"

MeshRender GameCore::m_MeshRender;
void   GameCore::CoreInit()
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

	DxState::Create();
	I_Shader.Init();
	if (I_Shader.g_pDefaultShader != nullptr)
	{
		I_InputLayout.Init(I_Shader.g_pDefaultShader->m_pCode.Get());
	}
	TLoadResData data;
	data.texPathName = L"../../data/texture/white.png";
	data.texShaderName = L"../../data/shader/DefaultMask.txt";
	m_MeshRender.Create(data);
	Init();
}
void   GameCore::CoreFrame()
{

	m_GameTimer.Frame();
	m_Input.Frame();
	m_DxWrite.Frame();
	m_DxDevice.Frame();


	if (m_Input.KeyCheck('V') == KEY_PUSH)
	{
		Device::m_bWireFrame = !Device::m_bWireFrame;
	}

	Frame();
}
void   GameCore::CoreRender()
{
	m_DxDevice.PreRender();
	{
		Render();

		m_DxWrite.PreRender();
		{
			D2D1_RECT_F rt = { 1000.0f, 0.0f, 1280.0f, 600.0f };
			m_DxWrite.Draw(rt, m_GameTimer.m_szTime);
			/*rt.top += 25.0f;
			m_DxWrite.Draw(rt, m_Input.m_szTime,
				D2D1::ColorF(1, 0, 0, 1));*/

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
void   GameCore::CoreRelease()
{
	m_GameTimer.Release();
	m_DxDevice.Release();
	m_Input.Release();
	m_DxWrite.Release();

	DxState::Release();

	m_MeshRender.Release();
	Release();
}
bool GameCore::GameRun()
{
	CoreInit();
	while (m_bRun)
	{
		if (!MessageProcess())
		{
			CoreFrame();
			CoreRender();
		}

	}
	CoreRelease();
	return true;
}