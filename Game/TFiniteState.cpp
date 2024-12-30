#include "TFiniteState.h"
void TFiniteState::AddTransition(UINT iEvent, UINT outputState)
{
	m_ActionMap[iEvent] = outputState;
}
void TFiniteState::DelTransition(UINT iEvent)
{
	m_ActionMap.erase(iEvent);
}
UINT TFiniteState::Output(UINT iEvent)
{
	auto iter = m_ActionMap.find(iEvent);
	if (iter != m_ActionMap.end())
	{
		return m_ActionMap[iEvent];
	}
	_ASSERT(0);
}
TFiniteState::TFiniteState(UINT iState)
{
	m_iActionState = iState;
}
TFiniteState::~TFiniteState()
{}

void TFiniteStateMachine::AddStateTransition(UINT iState, UINT iEvent, UINT iOutput)
{
	tFState pState = nullptr;
	auto iter = m_StateList.find(iState);
	if (iter != m_StateList.end())
	{
		pState = iter->second;
	}
	else
	{
		pState = std::make_shared<TFiniteState>(iState);
		m_StateList[pState->Get()] = pState;
	}
	if (pState)
	{
		pState->AddTransition(iEvent, iOutput);
	}
}
void TFiniteStateMachine::DelStateTransition(UINT iState, UINT iEvent)
{

}
UINT TFiniteStateMachine::GetOutputState(UINT iEvent)
{
	return 0;
}
UINT TFiniteStateMachine::GetOutputState(UINT iState, UINT iEvent)
{
	auto iter = m_StateList.find(iState);
	if (iter != m_StateList.end())
	{
		return iter->second->Output(iEvent);
	}
	_ASSERT(0);
}
TFiniteStateMachine::TFiniteStateMachine()
{

}

TFiniteStateMachine::~TFiniteStateMachine()
{

}