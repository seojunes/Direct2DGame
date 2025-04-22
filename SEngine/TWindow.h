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
	//���콺 �� ��ȭ��.
	short		m_nMouseWheelDelta = 0;
public:
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// ������Ŭ���� ���
	bool   SetWindowClass(HINSTANCE hInstance);
	// ������â ����
	bool   SetWindow(
		std::wstring title = L"Sample Project",
		UINT iWindowX = 800,
		UINT iWindowY = 600);
	// �޼��� ���ν���, ó��
	bool   MessageProcess();

public:
	TWindow();
};

