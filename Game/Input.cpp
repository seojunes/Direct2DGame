#include "Input.h"
TGameKey g_GameKey;
POINT    g_ptMouse;
DWORD   Input::KeyCheck(DWORD dwKey)
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
void    Input::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
}
void    Input::Frame()
{
	GetCursorPos(&m_ptMouse);			// 화면좌표
	ScreenToClient(g_hWnd, &m_ptMouse); // 클라이언트좌표
	g_ptMouse = m_ptMouse;
	// 키 매핑
	g_GameKey.dwWkey = KeyCheck('W');
	g_GameKey.dwSkey = KeyCheck('S');
	g_GameKey.dwAkey = KeyCheck('A');
	g_GameKey.dwDkey = KeyCheck('D');
	g_GameKey.dwPkey = KeyCheck('P');
	g_GameKey.dwKkey = KeyCheck('K');
	g_GameKey.dwLeftClick = KeyCheck(VK_LBUTTON);
	g_GameKey.dwRightClick = KeyCheck(VK_RBUTTON);
	g_GameKey.dwMiddleClick = KeyCheck(VK_MBUTTON);
	g_GameKey.dwExit = KeyCheck(VK_ESCAPE);
	g_GameKey.dwSpace = KeyCheck(VK_SPACE);
	g_GameKey.dwLeftbutton = KeyCheck(VK_LEFT);
	g_GameKey.dwRightbutton = KeyCheck(VK_RIGHT);
	g_GameKey.dwUpbutton = KeyCheck(VK_UP);
	g_GameKey.dwDownbutton = KeyCheck(VK_DOWN);
	g_GameKey.dw1key = KeyCheck(VK_NUMPAD1);
	g_GameKey.dw2key = KeyCheck(VK_NUMPAD2);
	g_GameKey.dw3key = KeyCheck(VK_NUMPAD3);
	g_GameKey.dw4key = KeyCheck(VK_NUMPAD4);




#ifdef _DEBUG
	m_szTime = std::to_wstring(m_ptMouse.x);
	m_szTime += L" ";
	m_szTime += std::to_wstring(m_ptMouse.y);
	m_szTime += L"\n";
#endif
}
void    Input::Render()
{
	/*m_fTmpTimer += g_fSPF;
	if (m_fTmpTimer > 1.0)
	{
#ifdef _DEBUG
		m_szTime = std::to_wstring(m_ptMouse.x);
		m_szTime += L" ";
		m_szTime += std::to_wstring(m_ptMouse.y);
		m_szTime += L"\n";
#endif
		m_fTmpTimer -= 1.0;
		OutputDebugString(m_szTime.c_str());
	}*/
}
void    Input::Release() {

}