#pragma once
#include "TMapObj.h"
#include "TFiniteState.h"
// STATE_STAND -> EVENT_PATROL (시간경과)    -> STATE_MOVE
// STATE_STAND -> EVENT_FINDTARGET(검색범위) -> STATE_ATTACK
// STATE_MOVE  -> EVENT_STOP		->STATE_STAND
// STATE_MOVE  -> EVENT_LOSTTARGET  ->STATE_STAND
// STATE_MOVE  -> EVENT_FINDTARGET  ->STATE_ATTACK
// STATE_ATTACK -> EVENT_LOSTTARGET ->STATE_STAND 

enum TActionState
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_COUNT,
};
// 상태전이
enum TActionEvent
{
	EVENT_FINDTARGET = 0,
	EVENT_LOSTTARGET,
	EVENT_STOP,
	EVENT_PATROL,
	EVENT_COUNT,
};

class TNpcObj;
class TEnemyState
{
public:
	UINT   m_iState;
	TEnemyState(TNpcObj* p);
	TEnemyState() {};
	virtual ~TEnemyState();
public:
	virtual void ProcessAction(TObject* pObj) = 0;
public:
	TNpcObj* m_pOwner;
};
class TStandAction : public TEnemyState
{
public:
	virtual void ProcessAction(TObject* pObj);
public:
	TStandAction(TNpcObj* p);
	TStandAction() { m_iState = STATE_STAND; };
	virtual ~TStandAction();
};
class TMoveAction : public TEnemyState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TMoveAction(TNpcObj* p);
	TMoveAction() { m_iState = STATE_MOVE; };
	virtual ~TMoveAction();
};
class TAttackAction : public TEnemyState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TAttackAction(TNpcObj* p);
	TAttackAction() { m_iState = STATE_ATTACK; };
	virtual ~TAttackAction();
};

