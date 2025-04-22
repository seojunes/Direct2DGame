#pragma once
#include "TStd.h"
// 1�� ������ �޽���
// 2�� ������ API(�Լ�)

class TInput
{
public:
	// Ű����, ���콺
	DWORD    m_dwKeyState[256];// ��� Ű���� �ش� ��ȣ�� �ִ�.
	POINT    m_ptMouse;
	POINT    m_ptDeltaMouse;
	POINT    m_ptDragStart;
	std::wstring	m_szTime;
	bool	m_bDrag = false;
	float	m_fTmpTimer = 0.0f;
public:
	DWORD    KeyCheck(DWORD dwKey);
public:
	virtual void    Init();		// �ʱ�ȭ �۾�
	virtual void	Reset();
	virtual void    Frame();	// ��� �۾�
	virtual void    Render();	// ȭ�翡 ��ο� �۾�
	virtual void    Release();	// �Ҹ�,���� �۾�
};

