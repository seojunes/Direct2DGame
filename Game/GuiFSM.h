#pragma once
#include "Object2D.h"
#include "FiniteState.h"

enum SelectState
{
	T_DEFAULT = 0,  // 커서가 위에 없을 때(T_FOCUS상태에서 다른 곳을 T_ACTIVE하면 전환된다.)
	T_HOVER,// = 1,	// 커서가 위에 있을 때
	//T_FOCUS,// = 2,	// T_ACTIVE상태에서 왼쪽 버튼을 다른 곳에서 놓았을 때(취소)
	T_ACTIVE,// = 4,	// 마우스 왼쪽 버튼 누르고 있을 때
	T_SELECTED,// = 8, // T_ACTIVE 상태에서 왼쪼버튼 놓았을 때
	T_COUNTER,
};
enum SelectEvent
{
	EVENT_DEFAULT = 0,
	EVENT_SELECT,
	EVENT_SELECT_COUNT,
};

class ControlGUI;
class GuiState
{
public:
	UINT   m_iState = 0;
	GuiState(ControlGUI* p);
	GuiState() {};
	virtual ~GuiState();
public:
	virtual void ProcessAction(Object* pObj) = 0;
public:
	ControlGUI* m_pOwner;
};

class DefaultActionGui : public GuiState
{
public:
	virtual void ProcessAction(Object* pObj);
public:
	DefaultActionGui(ControlGUI* p);
	DefaultActionGui() { m_iState = SelectState::T_DEFAULT; };
	virtual ~DefaultActionGui();
};

class HoverActionGui : public GuiState
{
public:
	virtual void ProcessAction(Object* pObj);
	HoverActionGui(ControlGUI* p);
	HoverActionGui() { m_iState = SelectState::T_HOVER; };
	virtual ~HoverActionGui();
};

class ActiveActionGui : public GuiState
{
public:
	virtual void ProcessAction(Object* pObj);
	ActiveActionGui(ControlGUI* p);
	ActiveActionGui() { m_iState = SelectState::T_ACTIVE; };
	virtual ~ActiveActionGui();
};

class SelectedActionGui : public GuiState
{
public:
	virtual void ProcessAction(Object* pObj);
	SelectedActionGui(ControlGUI* p);
	SelectedActionGui() { m_iState = SelectState::T_SELECTED; };
	virtual ~SelectedActionGui();
};


