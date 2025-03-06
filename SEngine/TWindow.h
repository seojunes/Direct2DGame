#pragma once
#include "TStd.h"
class TWindow
{
public:
	HINSTANCE   m_hInstance;
	HWND		m_hWnd;
	bool		m_bRun = true;
	POINT		m_ptClientSize;
	RECT		m_rtWindow;
	RECT		m_rtClient;
public:
	// ������Ŭ���� ���
	bool   SetWindowClass(HINSTANCE hInstance);
	// ������â ����
	bool   SetWindow(
		std::wstring title = L"Sample Project",
		UINT iWindowX=800,
		UINT iWindowY=600);
	// �޼��� ���ν���, ó��
	bool   MessageProcess();
};

