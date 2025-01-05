#pragma once
#include "TMapObj.h"
#include "TFiniteState.h"
class TMissileObj;
class TMissileState
{
public:
	UINT   m_iMissileState;
	TMissileState(TMissileObj* p);
	TMissileState() {};
	virtual ~TMissileState();
public:
	virtual void ProcessAction(TObject* pObj) = 0;
public:
	TMissileObj* m_pOwner;
};

class TIdleAction : public TMissileState
{
public:
	virtual void ProcessAction(TObject* pObj);
public:
	TIdleAction(TMissileObj* p);
	TIdleAction() { m_iMissileState = STATE_STAND; };
	virtual ~TIdleAction();
};
class TShotAction : public TMissileState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TShotAction(TMissileObj* p);
	TShotAction() { m_iMissileState = STATE_MOVE; };
	virtual ~TShotAction();
};
class TFlyingAction : public TMissileState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TFlyingAction(TMissileObj* p);
	TFlyingAction() { m_iMissileState = STATE_ATTACK; };
	virtual ~TFlyingAction();
};
class THitAction : public TMissileState
{
public:
	virtual void ProcessAction(TObject* pObj);
	THitAction(TMissileObj* p);
	THitAction() { m_iMissileState = STATE_ATTACK; };
	virtual ~THitAction();
};
class TExplodeAction : public TMissileState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TExplodeAction(TMissileObj* p);
	TExplodeAction() { m_iMissileState = STATE_ATTACK; };
	virtual ~TExplodeAction();
};
class TDestroyAction : public TMissileState
{
public:
	virtual void ProcessAction(TObject* pObj);
	TDestroyAction(TMissileObj* p);
	TDestroyAction() { m_iMissileState = STATE_ATTACK; };
	virtual ~TDestroyAction();
};


