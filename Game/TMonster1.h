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
	INT m_Hp = 100;


public:
	TMonster1()
	{
		m_fSpeed = 100.0f;
	}
};

