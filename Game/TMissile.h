#pragma once
#include "THeroObj.h"
#include "TMissileFSM.h"

struct TMissileStateData
{
	float m_fTimer;					// 상태 지속 시간
	float m_fSpeed;					// 이동 속도
	TVector2 m_vDirection;			// 이동 방향
	TVector2 m_vPosition;			// 현재 위치
	TVector2 m_TargetPosition;		// 목표 위치
	float m_fExplosionRadius;		// 폭발 반경 (폭발 상태에서 사용)
	float m_fExplosionDamage;		// 폭발 피해량 (폭발 상태에서 사용)
	bool m_bHasHit = false;			// 충돌 여부
	bool m_bIsDestroyed;			// 소멸 여부
};
class TMissileObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;
	THeroObj* m_pHero = nullptr;
public:
	TMissileState* m_pMAction = nullptr;
	std::vector<TMissileStateData>   m_MStateData;
	static std::vector<std::shared_ptr<TMissileState>>  m_pMActionList;
	TFiniteStateMachine* m_pMFsm = nullptr;
public:
	bool			m_bDead = false;
public:
	void SetFSM(TFiniteStateMachine* pMFsm);
	void SetTransition(UINT iEvent);
	static void StartFSM();
	void FrameState(TObject* pNpc);

	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void SetHero(THeroObj* pHero) { m_pHero = pHero; }
	void SetDirrection(HeroView view);
	virtual void Frame() override;
	virtual void SetVertexData();
	void    HitOverlap(TObject* pObj, THitResult hRes) override;

	TMissileObj()
	{
		m_fSpeed = 400.0f;
		m_vDir.x = 1.0f;
	}
};

