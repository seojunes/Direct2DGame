#pragma once
#include "TStd.h"

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




