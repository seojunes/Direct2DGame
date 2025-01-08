#pragma once
#include "TNpcObj.h"

enum class Monster1State
{
	STATE_Move = 0,
	STATE_Attack,
};
class TMonster1 : public TNpcObj
{
public:
	void SetVertexData() override;
	void Frame() override;
public:
	Monster1State m_state = Monster1State::STATE_Move;
public:
	TVector2 m_vInitedPos;
	UINT	 m_iLimitedDis = 300.0f;
public:
	/*TMonster1()
	{
		TNpcObj::m_HP = 10;
		m_fSpeed = 100.0f;
		m_vInitedPos = m_rtScreen.vc;
	}*/
	TMonster1(const TVector2& initialPos)
	{
		TNpcObj::m_HP = 10;
		m_fSpeed = 100.0f;
		m_vInitedPos = initialPos; // 초기 위치 설정
		m_vPos = initialPos;       // 현재 위치도 초기 위치로 설정
	}
};

class TMonster2 : public TNpcObj
{
public:
	void SetVertexData() override;
	void Frame() override;
public:
	TMonster2()
	{
		TNpcObj::m_HP = 30;
		m_fSpeed = 100.0f;
	}
};

class TMonster3 : public TNpcObj
{
public:
	void SetVertexData() override;
	void Frame() override;
public:
	TMonster3()
	{
		TNpcObj::m_HP = 15;
		m_fSpeed = 100.0f;
	}
};
