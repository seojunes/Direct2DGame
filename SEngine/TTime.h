#pragma once
#include <chrono> // �ð����� ���
#include <string>
using namespace std::chrono;

class TBaseClass
{
	virtual void    Init() = 0;		// �ʱ�ȭ �۾�
	virtual void    Frame() = 0;	// ��� �۾�
	virtual void    Render() = 0;	// ȭ�翡 ��ο� �۾�
	virtual void    Release() = 0;	// �Ҹ�,���� �۾�
};

class TTime : public TBaseClass
{
public:
	int		m_iTmpGameFrame = 0;//1�ʿ� ���ư� ������ ����
	int		m_iGameFrame = 0;//1�ʿ� ���ư� ������ ����.
	float   m_fGameTimer = 0.0f;// ���� ���� �ð� ����
	float   m_fSecondPerFrame = 0.0f;// 1�������� ����ð�
	std::wstring	m_szTime;
	system_clock::time_point	m_StartClock;
	system_clock::time_point	m_EndClock;
	float			m_fTmpTimer;
public:
	virtual void    Init();		// �ʱ�ȭ �۾�
	virtual void    Frame();	// ��� �۾�
	virtual void    Render();	// ȭ�翡 ��ο� �۾�
	virtual void    Release();	// �Ҹ�,���� �۾�
};
class TEventTime : public TTime
{
public:
	float   m_fElapseTime = 0.0f;
};



//class TTime : public TBaseClass
//{
//public:
//	int		m_iGameFrame = 0;//1�ʿ� ���ư� ������ ����.
//	float   m_fGameTimer = 0.0f;// ���� ���� �ð� ����
//	float   m_fSecondPerFrame = 0.0f;// 1�������� ����ð�
//	std::wstring	m_szTime;
//	std::clock_t	m_StartClock;
//	std::clock_t	m_EndClock;
//	float			m_fTmpTimer;
//public:
//	virtual void    Init();		// �ʱ�ȭ �۾�
//	virtual void    Frame();	// ��� �۾�
//	virtual void    Render();	// ȭ�翡 ��ο� �۾�
//	virtual void    Release();	// �Ҹ�,���� �۾�
//};