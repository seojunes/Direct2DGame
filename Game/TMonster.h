#pragma once
#include "TNpcObj.h"

enum class Monster1State
{
	STATE_Move = 0,
	STATE_Attack,
	STATE_Return,
};

//enum class Monster1Event
//{
//	EVENT_Findtarget = 0,
//	EVENT_Losstarget,
//	EVENT_BeAttacked,
//};
class TMonster1 : public TNpcObj
{
public:
	void SetVertexData() override;
	void Frame() override;
public:
	Monster1State m_state = Monster1State::STATE_Move;
public:
	TVector2 m_vInitedPos;
	UINT	 m_iLimitedDis = 150.0f;
public:
	/*TMonster1()
	{
		TNpcObj::m_HP = 10;
		m_fSpeed = 100.0f;
		m_vInitedPos = m_rtScreen.vc;
	}*/
	TMonster1(const TVector2& initialPos)
	{
		TNpcObj::m_HP = 50;
		m_fSpeed = 100.0f;
		m_vInitedPos = initialPos; // 초기 위치 설정
		m_vPos = initialPos;       // 현재 위치도 초기 위치로 설정
	}
};


enum class Monster2State
{
	STATE_Idle = 0,
	STATE_Attack,
};

class TMonster2 : public TNpcObj
{

public:
	void SetVertexData() override;
	virtual void Init() override;
	virtual void Frame() override;
	virtual void Render()override;
	virtual void Release()override;
public:
	Monster2State m_state = Monster2State::STATE_Idle;
public:
	float m_ftrigger = 0.5f;
	//void shot();
public:
	TMonster2()
	{
		TNpcObj::m_HP = 30;
		m_fSpeed = 100.0f;
	}
};

enum class Monster3State
{
	STATE_Idle = 0,
	STATE_Attack,
};

class TMonster3 : public TNpcObj
{
public:
	void SetVertexData() override;
	virtual void Init() override;
	virtual void Frame() override;
	virtual void Render()override;
	virtual void Release()override;
public:
	Monster2State m_state = Monster2State::STATE_Idle;
public:
	float m_ftrigger = 1.0f;
public:
	TMonster3()
	{
		TNpcObj::m_HP = 15;
		m_fSpeed = 100.0f;
	}
};
