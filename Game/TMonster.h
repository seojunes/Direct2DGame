#pragma once
#include "TNpcObj.h"

enum class Monster1State
{
	STATE_Move =0,
	STATE_Attack,
	STATE_Return,
	STATE_Dead,
};


class TMonster1 : public TNpcObj
{
public:
	void SetVertexData() override;
	void Frame() override;
public:
	Monster1State m_state = Monster1State::STATE_Move;
public:
	//float	 m_fCurrentTime = 0.0f;
	TVector2 m_vInitedPos;
	UINT	 m_iLimitedDis = 150.0f;
	INT		 m_iPreHP = 0;
	float    m_fFindTime = 0.5f;
	float	 m_fCurrentTime = 0.0f;
	
	bool     m_fDeadCheck = false;
public:
	TMonster1(const TVector2& initialPos)
	{
		TNpcObj::m_HP = m_iPreHP = 40;
		m_fSpeed = 100.0f;
		m_vInitedPos = initialPos; // 초기 위치 설정
		m_vPos = initialPos;       // 현재 위치도 초기 위치로 설정
	}
};


enum class Monster2State
{
	STATE_Idle = 0,
	STATE_Attack,
	STATE_Dead,
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
	float	 m_fCurrentTime = 0.0f;
	
	//void shot();
public:
	TMonster2()
	{
		TNpcObj::m_HP = 20;
		m_fSpeed = 100.0f;
	}
};

enum class Monster3State
{
	STATE_Idle = 0,
	STATE_Attack,
	STATE_Dead,
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
	Monster3State m_state = Monster3State::STATE_Idle;
public:
	float m_ftrigger = 0.3f;
	float	 m_fCurrentTime = 0.0f;
public:
	TMonster3()
	{
		TNpcObj::m_HP = 30;
		m_fSpeed = 100.0f;
	}
};
