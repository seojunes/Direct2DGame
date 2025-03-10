#include "TInput.h"
TGameKey g_GameKey;
POINT    g_ptMouse;
POINT    g_ptDeltaMouse;
DWORD   TInput::KeyCheck(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);
	// 0x8000 : 현재 눌러져 있다.
	// 0x0001 : 눌러진 적이 있다.
	// 0x8001 : 눌러진 적이 있고 현재 눌러졌다.
	// 0X0000 : 눌러지지 않았다.

	if (sKey & 0x8000 || sKey & 0x8001) // 1000 0000 0000 0000
	{
		if (m_dwKeyState[dwKey] == KEY_FREE ||
			m_dwKeyState[dwKey] == KEY_UP)
			m_dwKeyState[dwKey] = KEY_PUSH;
		else
			m_dwKeyState[dwKey] = KEY_HOLD;
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_PUSH ||
			m_dwKeyState[dwKey] == KEY_HOLD)
			m_dwKeyState[dwKey] = KEY_UP;
		else
			m_dwKeyState[dwKey] = KEY_FREE;
	}
	return m_dwKeyState[dwKey];
}
void    TInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	GetCursorPos(&m_ptMouse);			// 화면좌표
	ScreenToClient(g_hWnd, &m_ptMouse); // 클라이언트좌표
	g_ptMouse = m_ptMouse;
}
void    TInput::Frame()
{
	GetCursorPos(&m_ptMouse);			// 화면좌표
	ScreenToClient(g_hWnd, &m_ptMouse); // 클라이언트좌표
	POINT ptDeltaMouse;
	ptDeltaMouse.x = g_ptMouse.x - m_ptMouse.x;
	ptDeltaMouse.y = g_ptMouse.y - m_ptMouse.y;
	g_ptMouse = m_ptMouse;
	// 키 매핑
	g_GameKey.dwWkey = KeyCheck('W');
	g_GameKey.dwSkey = KeyCheck('S');
	g_GameKey.dwAkey = KeyCheck('A');
	g_GameKey.dwDkey = KeyCheck('D');
	g_GameKey.dw0key = KeyCheck(VK_NUMPAD0);
	g_GameKey.dw1key = KeyCheck(VK_NUMPAD1);
	g_GameKey.dw2key = KeyCheck(VK_NUMPAD2);
	g_GameKey.dw3key = KeyCheck(VK_NUMPAD3);
	g_GameKey.dw4key = KeyCheck(VK_NUMPAD4);
	g_GameKey.dw5key = KeyCheck(VK_NUMPAD5);
	g_GameKey.dw6key = KeyCheck(VK_NUMPAD6);
	g_GameKey.dw7key = KeyCheck(VK_NUMPAD7);
	g_GameKey.dw8key = KeyCheck(VK_NUMPAD8);
	g_GameKey.dw9key = KeyCheck(VK_NUMPAD9);
	g_GameKey.dwLeftClick = KeyCheck(VK_LBUTTON);
	g_GameKey.dwRightClick = KeyCheck(VK_RBUTTON);
	g_GameKey.dwMiddleClick = KeyCheck(VK_MBUTTON);
	g_GameKey.dwExit = KeyCheck(VK_ESCAPE);
	g_GameKey.dwSpace = KeyCheck(VK_SPACE);
	if (g_GameKey.dwLeftClick == KEY_PUSH)
	{
		m_ptDragStart = m_ptMouse;
		m_bDrag = true;
	}
	if (g_GameKey.dwLeftClick == KEY_UP)
	{
		m_bDrag = false;
		g_ptDeltaMouse.x = 0.0f;
		g_ptDeltaMouse.y = 0.0f;
	}
	if (m_bDrag)
	{
		//g_ptDeltaMouse.x = m_ptDragStart.x - m_ptMouse.x;
		//g_ptDeltaMouse.y = m_ptDragStart.y - m_ptMouse.y;
		g_ptDeltaMouse.x = ptDeltaMouse.x;
		g_ptDeltaMouse.y = ptDeltaMouse.y;
	}

	//#ifdef _DEBUG
	//		m_szTime = std::to_wstring(g_ptDeltaMouse.x);
	//		m_szTime += L" ";
	//		m_szTime += std::to_wstring(g_ptDeltaMouse.y);
	//		m_szTime += L"\n";
	//#endif
}
void    TInput::Render()
{
	m_fTmpTimer += g_fSPF;
	//if (m_fTmpTimer > 1.0)
	{
#ifdef _DEBUG
		m_szTime = std::to_wstring(g_ptDeltaMouse.x);
		m_szTime += L" ";
		m_szTime += std::to_wstring(g_ptDeltaMouse.y);
		m_szTime += L"\n";
#endif
		m_fTmpTimer -= 1.0;
		OutputDebugString(m_szTime.c_str());
	}
}
void    TInput::Release() {

}