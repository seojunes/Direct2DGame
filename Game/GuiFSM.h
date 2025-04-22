#pragma once
#include "Object2D.h"
#include "FiniteState.h"

enum SelectState
{
	T_DEFAULT = 0,  // Ŀ���� ���� ���� ��(T_FOCUS���¿��� �ٸ� ���� T_ACTIVE�ϸ� ��ȯ�ȴ�.)
	T_HOVER,// = 1,	// Ŀ���� ���� ���� ��
	//T_FOCUS,// = 2,	// T_ACTIVE���¿��� ���� ��ư�� �ٸ� ������ ������ ��(���)
	T_ACTIVE,// = 4,	// ���콺 ���� ��ư ������ ���� ��
	T_SELECTED,// = 8, // T_ACTIVE ���¿��� ���ɹ�ư ������ ��
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


