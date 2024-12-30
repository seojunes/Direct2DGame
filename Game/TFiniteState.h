#pragma once
#include "TStd.h"
// STATE_STAND -> EVENT_PATROL (시간경과)    -> STATE_MOVE
// STATE_STAND -> EVENT_FINDTARGET(검색범위) -> STATE_ATTACK
// STATE_MOVE  -> EVENT_STOP		->STATE_STAND
// STATE_MOVE  -> EVENT_LOSTTARGET  ->STATE_STAND
// STATE_MOVE  -> EVENT_FINDTARGET  ->STATE_ATTACK
// STATE_ATTACK -> EVENT_LOSTTARGET ->STATE_STAND 

enum T_ActionState
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_COUNT,
};
// 상태전이
enum T_ActionEvent
{
	EVENT_FINDTARGET = 0,
	EVENT_LOSTTARGET,
	EVENT_STOP,
	EVENT_PATROL,
	EVENT_COUNT,
};
// 유한상태
class TFiniteState
{
public:
	UINT		m_iActionState;
	//       Event->ResultState
	std::map<UINT, UINT>  m_ActionMap;
	void AddTransition(UINT iEvent, UINT outputState);
	void DelTransition(UINT iEvent);
	UINT Output(UINT iEvent);
	UINT Get() { return m_iActionState; }
public:
	TFiniteState(UINT iState);
	virtual ~TFiniteState();
};
class TFiniteStateMachine
{
public:
	using tFState = std::shared_ptr<TFiniteState>;
	//      엑션상태, 해당상태객체
	std::map<UINT, tFState>  m_StateList;
	void AddStateTransition(UINT iState, UINT iEvent, UINT iOutput);
	void DelStateTransition(UINT iState, UINT iEvent);
	UINT GetOutputState(UINT iEvent);
	UINT GetOutputState(UINT iState, UINT iEvent);
public:
	TFiniteStateMachine();
	virtual ~TFiniteStateMachine();
};

