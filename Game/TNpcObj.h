#pragma once
#include "TEnemyFSM.h"
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
	TMapObj* m_pMap = nullptr;
	TEnemyState* m_pAction = nullptr;
	std::vector<TStateData>   m_StateData;
	static std::vector<std::shared_ptr<TEnemyState>>  m_pActionList;
	TFiniteStateMachine* m_pFsm = nullptr;
public:
	void SetFSM(TFiniteStateMachine* pFsm);
	void SetTransition(UINT iEvent);
	static void CreateActionFSM();
	void FrameState(TObject* pHero);
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

