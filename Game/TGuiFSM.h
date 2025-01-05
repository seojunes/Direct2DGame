#pragma once
#include "TObject2D.h"
#include "TFiniteState.h"

enum TSelectState
{
	T_DEFAULT = 0,  // Ŀ���� ���� ���� ��(T_FOCUS���¿��� �ٸ� ���� T_ACTIVE�ϸ� ��ȯ�ȴ�.)
	T_HOVER,// = 1,	// Ŀ���� ���� ���� ��
	//T_FOCUS,// = 2,	// T_ACTIVE���¿��� ���� ��ư�� �ٸ� ������ ������ ��(���)
	T_ACTIVE,// = 4,	// ���콺 ���� ��ư ������ ���� ��
	T_SELECTED,// = 8, // T_ACTIVE ���¿��� ���ɹ�ư ������ ��
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


