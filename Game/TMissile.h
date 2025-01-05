#pragma once
#include "THeroObj.h"
#include "TMissileFSM.h"

struct TMissileStateData
{
	float m_fTimer;					// ���� ���� �ð�
	float m_fSpeed;					// �̵� �ӵ�
	TVector2 m_vDirection;			// �̵� ����
	TVector2 m_vPosition;			// ���� ��ġ
	TVector2 m_TargetPosition;		// ��ǥ ��ġ
	float m_fExplosionRadius;		// ���� �ݰ� (���� ���¿��� ���)
	float m_fExplosionDamage;		// ���� ���ط� (���� ���¿��� ���)
	bool m_bHasHit = false;			// �浹 ����
	bool m_bIsDestroyed;			// �Ҹ� ����
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

