#pragma once
#include "TObject2D.h"
#include "TFiniteState.h"

enum TSelectState
{
	T_DEFAULT = 0,  // 커서가 위에 없을 때(T_FOCUS상태에서 다른 곳을 T_ACTIVE하면 전환된다.)
	T_HOVER,// = 1,	// 커서가 위에 있을 때
	//T_FOCUS,// = 2,	// T_ACTIVE상태에서 왼쪽 버튼을 다른 곳에서 놓았을 때(취소)
	T_ACTIVE,// = 4,	// 마우스 왼쪽 버튼 누르고 있을 때
	T_SELECTED,// = 8, // T_ACTIVE 상태에서 왼쪼버튼 놓았을 때
	T_COUNTER,
};
enum TSelectEvent
{
	EVENT_DEFAULT = 0,
	EVENT_SELECT,
	EVENT_SELECT_COUNT,
};

class TControlGUI;
class TGuiState
{
public:
	UINT   m_iState = 0;
	TGuiState(TControlGUI* p);
	TGuiState() {};
	virtual ~TGuiState();
public:
	virtual void ProcessAction(TObject* pObj) = 0;
public:
	TControlGUI* m_pOwner;
};
class TDefaultActionGui : public TGuiState
{
public:
	virtual void ProcessAction(TObject* pObj);
public:
	TDefaultActionGui(TControlGUI* p);
	TDefaultActionGui() { m_iState = TSelectState::T_DEFAULT; };
	virtual ~TDefaultActionGui();
};
class THoverActionGui : public TGuiState
{
public:
	virtual void ProcessAction(TObject* pObj);
	THoverActionGui(TControlGUI* p);
	THoverActionGui() { m_iState = TSelectState::T_HOVER; };
	virtual ~THoverActionGui();
};
class TActiveActionGui : public TGuiState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TActiveActionGui(TControlGUI* p);
	TActiveActionGui() { m_iState = TSelectState::T_ACTIVE; };
	virtual ~TActiveActionGui();
};
class TSelectedActionGui : public TGuiState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TSelectedActionGui(TControlGUI* p);
	TSelectedActionGui() { m_iState = TSelectState::T_SELECTED; };
	virtual ~TSelectedActionGui();
};


