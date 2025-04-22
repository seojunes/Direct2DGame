#include "FiniteState.h"
void FiniteState::AddTransition(UINT iEvent, UINT outputState)
{
	m_ActionMap[iEvent] = outputState;
}
void FiniteState::DelTransition(UINT iEvent)
{
	m_ActionMap.erase(iEvent);
}
UINT FiniteState::Output(UINT iEvent)
{
	auto iter = m_ActionMap.find(iEvent);
	if (iter != m_ActionMap.end())
	{
		return m_ActionMap[iEvent];
	}
	_ASSERT(0);
}
FiniteState::FiniteState(UINT iState)
{
	m_iActionState = iState;
}
FiniteState::~FiniteState()
{}

void FiniteStateMachine::AddStateTransition(UINT iState, UINT iEvent, UINT iOutput)
{
	tFState pState = nullptr;
	auto iter = m_StateList.find(iState);
	if (iter != m_StateList.end())
	{
		pState = iter->second;
	}
	else
	{
		pState = std::make_shared<FiniteState>(iState);
		m_StateList[pState->Get()] = pState;
	}
	if (pState)
	{
		pState->AddTransition(iEvent, iOutput);
	}
}
void FiniteStateMachine::DelStateTransition(UINT iState, UINT iEvent)
{

}
UINT FiniteStateMachine::GetOutputState(UINT iEvent)
{
	return 0;
}
UINT FiniteStateMachine::GetOutputState(UINT iState, UINT iEvent)
{
	auto iter = m_StateList.find(iState);
	if (iter != m_StateList.end())
	{
		return iter->second->Output(iEvent);
	}
	_ASSERT(0);
}
FiniteStateMachine::FiniteStateMachine()
{

}

FiniteStateMachine::~FiniteStateMachine()
{

}