#pragma once
#include <chrono> // 시간관련 기능
#include <string>
using namespace std::chrono;

class Time
{
public:
	int		m_iTmpGameFrame = 0;		//1초에 돌아간 프레임 측정
	int		m_iGameFrame = 0;			//1초에 돌아간 프레임 측정.
	float   m_fGameTimer = 0.0f;		// 게임 실행 시간 측정
	float   m_fSecondPerFrame = 0.0f;	// 1프레임의 경과시간
	std::wstring	m_szTime;
	system_clock::time_point	m_StartClock;
	system_clock::time_point	m_EndClock;
	float			m_fTmpTimer;
public:
	virtual void    Frame();	
	virtual void    Init();		
	virtual void    Render();	
	virtual void    Release();	
};
class TEventTime : public Time
{
public:
	float   m_fElapseTime = 0.0f;
};


