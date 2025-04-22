#pragma once
#include <chrono> // �ð����� ���
#include <string>
using namespace std::chrono;

class Time
{
public:
	int		m_iTmpGameFrame = 0;		//1�ʿ� ���ư� ������ ����
	int		m_iGameFrame = 0;			//1�ʿ� ���ư� ������ ����.
	float   m_fGameTimer = 0.0f;		// ���� ���� �ð� ����
	float   m_fSecondPerFrame = 0.0f;	// 1�������� ����ð�
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


