#pragma once
#include <chrono> // 시간관련 기능
#include <string>
using namespace std::chrono;

class TBaseClass
{
	virtual void    Init() = 0;		// 초기화 작업
	virtual void    Frame() = 0;	// 계산 작업
	virtual void    Render() = 0;	// 화며에 드로우 작업
	virtual void    Release() = 0;	// 소멸,삭제 작업
};

class TTime : public TBaseClass
{
public:
	int		m_iTmpGameFrame = 0;//1초에 돌아간 프레임 측정
	int		m_iGameFrame = 0;//1초에 돌아간 프레임 측정.
	float   m_fGameTimer = 0.0f;// 게임 실행 시간 측정
	float   m_fSecondPerFrame = 0.0f;// 1프레임의 경과시간
	std::wstring	m_szTime;
	system_clock::time_point	m_StartClock;
	system_clock::time_point	m_EndClock;
	float			m_fTmpTimer;
public:
	virtual void    Init();		// 초기화 작업
	virtual void    Frame();	// 계산 작업
	virtual void    Render();	// 화며에 드로우 작업
	virtual void    Release();	// 소멸,삭제 작업
};
class TEventTime : public TTime
{
public:
	float   m_fElapseTime = 0.0f;
};



//class TTime : public TBaseClass
//{
//public:
//	int		m_iGameFrame = 0;//1초에 돌아간 프레임 측정.
//	float   m_fGameTimer = 0.0f;// 게임 실행 시간 측정
//	float   m_fSecondPerFrame = 0.0f;// 1프레임의 경과시간
//	std::wstring	m_szTime;
//	std::clock_t	m_StartClock;
//	std::clock_t	m_EndClock;
//	float			m_fTmpTimer;
//public:
//	virtual void    Init();		// 초기화 작업
//	virtual void    Frame();	// 계산 작업
//	virtual void    Render();	// 화며에 드로우 작업
//	virtual void    Release();	// 소멸,삭제 작업
//};