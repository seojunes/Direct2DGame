#pragma once
#include "TNpcObj.h"

enum Monster1State
{
	STATE_Idel = 0,
	STATE_Move,
};
class TMonster1 : public TNpcObj
{
public:
	void SetVertexData() override;
public:
	TMonster1()
	{
		TNpcObj::m_HP = 10;
		m_fSpeed = 100.0f;
	}
};

class TMonster2 : public TNpcObj
{
public:
	void SetVertexData() override;
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
public:
	TMonster3()
	{
		TNpcObj::m_HP = 15;
		m_fSpeed = 100.0f;
	}
};
