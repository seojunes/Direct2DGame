#pragma once
#include "TEnemyFSM.h"
#include "THeroObj.h"
struct TStateData
{
	float m_fTimer;
	float m_fDefaultTimer;
	float m_fDistance;
};

class TEnemyFSM : public TFiniteStateMachine
{

};


class TNpcObj : public TObject2D
{
public:
	THeroObj* m_pHero = nullptr;
	TVector2 InitPos;
public:
	INT m_HP = 20;
	TMapObj* m_pMap = nullptr;
	TEnemyState* m_pAction = nullptr;
	std::vector<TStateData>   m_StateData;
	static std::vector<std::shared_ptr<TEnemyState>>  m_pActionList;
	TFiniteStateMachine* m_pFsm = nullptr;
public:
	std::shared_ptr<TProjectile>		m_pProjectile;
public:
	void SetFSM(TFiniteStateMachine* pFsm);
	void SetTransition(UINT iEvent);
	static void CreateActionFSM();
	void FrameState(TObject* pHero);
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	
	virtual void SetVertexData();
	void    HitOverlap(TObject* pObj, THitResult hRet) override;
	TNpcObj()
	{
		m_fSpeed = 150.0f;
		m_vDir.x = -1.0f;
		m_vDir.y = 0.0f;
	}
public:
	TObjectType GetType() const override
	{
		return TObjectType::Npc;
	}
};

