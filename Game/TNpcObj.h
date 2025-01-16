#pragma once
#include "TEnemyFSM.h"
#include "THeroObj.h"
struct TStateData
{
	float m_fTimer;
	float m_fDefaultTimer;
	float m_fDistance;
};

//enum ATTACKSTATE
//{
//	STATE_NONE = 0,
//	STATE_OK,
//};
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
	//ATTACKSTATE m_eAttacked = ATTACKSTATE::STATE_NONE;
public:
	void SetFSM(TFiniteStateMachine* pFsm);
	void SetTransition(UINT iEvent);
	static void CreateActionFSM();
	void FrameState(TObject* pHero);
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
public:
	std::vector<RECT> m_rtMon1AttackFrames; 
	std::vector<RECT> m_rtBossCreateFrames;
	std::vector<RECT> m_rtBossIdleFrames;
	std::vector<RECT> m_rtBossAttack1Frames;
	std::vector<RECT> m_rtBossTransFrames;
	std::vector<RECT> m_rtBoss2PhaseFrames;
	std::vector<RECT> m_rtBossM1Frames;
	std::vector<RECT> m_rtBossM2Frames;
	std::vector<RECT> m_rtBossFlyingFrames;
	UINT m_iMon1AttckFrame = 0;            
	UINT m_iBossCreateFrame = 0;
	UINT m_iBossIdleFrame = 0;
	UINT m_iBossAttack1Frame = 0;
	UINT m_iBossTransFrame = 0;
	UINT m_iBoss2PhaseFrame = 0;
	UINT m_iBossM1Frame = 0;
	UINT m_iBossM2Frame = 0;
	UINT m_iBossFlyingFrames = 0;
	float m_fMon1AttackFrameTime = 0.3f;
	float m_fBossCreateFrameTime = 0.2f;
	float m_fBossIdleFrameTime = 0.2f;
	float m_fBossAttack1FrameTime = 0.2f;
	float m_fBossTransFrameTime = 0.5f;
	float m_fBoss2PhaseFrameTime = 0.5f;
	float m_fBossM1FrameTime = 0.5f;
	float m_fBossM2FrameTime = 0.5f;
	float m_fBossFlyingFrameTime = 0.2f;

	
	bool m_bLoop = true;
	void SetData(vector<vector<RECT>> SpriteList);
public:
	HeroView m_eCurrentView = HeroView::LeftView;
public:
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
public:
	TVector2 Attacked(TVector2 pos);
	bool m_bAttacked = false;
};

