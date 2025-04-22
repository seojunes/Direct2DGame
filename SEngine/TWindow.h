#pragma once
#include "TStd.h"
class TWindow
{
public:
	HINSTANCE   m_hInstance;
	HWND		m_hWnd;
	bool		m_bRun = true;
	bool		m_bActive = true;
	POINT		m_ptClientSize;
	RECT		m_rtWindow;
	RECT		m_rtClient;
	//마우스 휠 변화값.
	short		m_nMouseWheelDelta = 0;
public:
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 윈도우클래스 등록
	bool   SetWindowClass(HINSTANCE hInstance);
	// 윈도우창 생성
	bool   SetWindow(
		std::wstring title = L"Sample Project",
		UINT iWindowX = 800,
		UINT iWindowY = 600);
	// 메세지 프로시쳐, 처리
	bool   MessageProcess();

public:
	TWindow();
};

