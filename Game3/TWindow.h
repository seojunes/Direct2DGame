#pragma once
#include "TStd.h"
class TWindow
{
public:
	HINSTANCE   m_hInstance;
	HWND		m_hWnd;
	bool		m_bRun = true;
	POINT		m_WindowSize;
public:
	// ������Ŭ���� ���
	bool   SetWindowClass(HINSTANCE hInstance);
	// ������â ����
	bool   SetWindow(
		std::wstring title = L"Sample Project",
		UINT iWindowX=1280,
		UINT iWindowY=800);
	// �޼��� ���ν���, ó��
	bool   MessageProcess();
};

