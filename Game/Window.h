#pragma once
#include "Std.h"
class Window
{
public:
	HINSTANCE   m_hInstance;
	HWND		m_hWnd;
	bool		m_bRun = true;
	POINT		m_ptClientSize;
	RECT		m_rtWindow;
	RECT		m_rtClient;
public:
	// 윈도우클래스 등록
	bool   SetWindowClass(HINSTANCE hInstance);
	// 윈도우창 생성
	bool   SetWindow(
		std::wstring title = L"Sample Project",
		UINT iWindowX = 1280,
		UINT iWindowY = 800);
	// 메세지 프로시쳐, 처리
	bool   MessageProcess();
};

