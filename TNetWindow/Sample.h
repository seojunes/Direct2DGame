#pragma once
//���� �� �̺�Ʈ
//copy "*.h" "..\\..\\TCore\\Inc"
//copy "..\\..\\output\\debug\\TCore.lib" "..\\..\\TCore\\lib\\"

#include "TGameCore.h"
#include "TNetwork.h"
class Sample : public TGameCore
{
	HWND   m_hEdit;
	HWND   m_hSendBtn;
	HWND   m_hListBtn;
	TNetwork m_Net;
public:
	void    Init();
	void    Frame();
	void    Render();
	void    Release();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

