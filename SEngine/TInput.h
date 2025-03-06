#pragma once
#include "TStd.h"
// 1번 윈도우 메시지
// 2번 윈도우 API(함수)

class TInput
{
public:
	// 키보드, 마우스
	DWORD    m_dwKeyState[256];// 모든 키들은 해당 번호가 있다.
	POINT    m_ptMouse;
	std::wstring	m_szTime;
	float	m_fTmpTimer = 0.0f;
public:
	DWORD    KeyCheck(DWORD dwKey);
public:
	virtual void    Init();		// 초기화 작업
	virtual void    Frame();	// 계산 작업
	virtual void    Render();	// 화며에 드로우 작업
	virtual void    Release();	// 소멸,삭제 작업
};

