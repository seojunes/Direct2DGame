#pragma once
#include "TMapObj.h"
#include "TFiniteState.h"
class TNpcObj;
class TEnemyState
{
public:
	UINT   m_iEnemyState;
	TEnemyState(TNpcObj* p);
	virtual ~TEnemyState();
public:
	virtual void ProcessAction(TObject* pObj) = 0;
public:
	TNpcObj* m_pOwner;
};
class TStandAction : public TEnemyState
{
	float   m_fTimer = 0.0f;
public:
	virtual void ProcessAction(TObject* pObj);
public:
	TStandAction(TNpcObj* p);
	virtual ~TStandAction();
};
class TMoveAction : public TEnemyState
{
	float m_fTimer = 0.0f;
public:
	virtual void ProcessAction(TObject* pObj);
	TMoveAction(TNpcObj* p);
	virtual ~TMoveAction();
};
class TAttackAction : public TEnemyState
{
	float m_fTimer = 0.0f;
public:
	virtual void ProcessAction(TObject* pObj);
	TAttackAction(TNpcObj* p);
	virtual ~TAttackAction();
};

class TNpcObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;
	TEnemyState* m_pAction = nullptr;
	std::vector<std::shared_ptr<TEnemyState>>  m_pActionList;
	TFiniteStateMachine* m_pFsm = nullptr;
public:
	void SetTransition(UINT iEvent)
	{
		_ASSERT(m_pFsm);
		UINT iOutput = m_pFsm->GetOutputState(
			m_pAction->m_iEnemyState, iEvent);
		m_pAction = m_pActionList[iOutput].get();
	}
	void StartFSM(TFiniteStateMachine* pFsm)
	{
		m_pFsm = pFsm;
		std::shared_ptr<TEnemyState> stand = std::make_shared<TStandAction>(this);
		std::shared_ptr<TEnemyState> move = std::make_shared<TMoveAction>(this);;
		std::shared_ptr<TEnemyState> attack = std::make_shared<TAttackAction>(this);;
		m_pActionList.emplace_back(stand);
		m_pActionList.emplace_back(move);
		m_pActionList.emplace_back(attack);
		m_pAction = stand.get();
	}
	void FrameState(TObject* pHero)
	{
		m_pAction->ProcessAction(pHero);
	}
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	virtual void Frame() override;
	virtual void SetVertexData();
	void    HitOverlap(TObject* pObj, THitResult hRet) override;
	TNpcObj()
	{
		m_fSpeed = 150.0f;
		m_vDir.x = 1.0f;
		m_vDir.y = 1.0f;
	}
};

