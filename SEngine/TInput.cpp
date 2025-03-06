#include "TInput.h"
TGameKey g_GameKey;
POINT    g_ptMouse;
DWORD   TInput::KeyCheck(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);
	// 0x8000 : ���� ������ �ִ�.
	// 0x0001 : ������ ���� �ִ�.
	// 0x8001 : ������ ���� �ְ� ���� ��������.
	// 0X0000 : �������� �ʾҴ�.

	if (sKey & 0x8000 || sKey& 0x8001) // 1000 0000 0000 0000
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
}
void    TInput::Frame() 
{
	GetCursorPos(&m_ptMouse);			// ȭ����ǥ
	ScreenToClient(g_hWnd, &m_ptMouse); // Ŭ���̾�Ʈ��ǥ
	g_ptMouse = m_ptMouse;
	// Ű ����
	g_GameKey.dwWkey		= KeyCheck('W');
	g_GameKey.dwSkey		= KeyCheck('S');
	g_GameKey.dwAkey		= KeyCheck('A');
	g_GameKey.dwDkey		= KeyCheck('D');
	g_GameKey.dwLeftClick	= KeyCheck(VK_LBUTTON);
	g_GameKey.dwRightClick	= KeyCheck(VK_RBUTTON);
	g_GameKey.dwMiddleClick = KeyCheck(VK_MBUTTON);
	g_GameKey.dwExit		= KeyCheck(VK_ESCAPE);
	g_GameKey.dwSpace		= KeyCheck(VK_SPACE);

#ifdef _DEBUG
		m_szTime = std::to_wstring(m_ptMouse.x);
		m_szTime += L" ";
		m_szTime += std::to_wstring(m_ptMouse.y);
		m_szTime += L"\n";
#endif
}
void    TInput::Render() 
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
void    TInput::Release() {

}