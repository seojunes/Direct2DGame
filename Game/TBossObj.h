#pragma once
#include "TNpcObj.h"

enum class BossState
{
	STATE_Idle = 0,
	STATE_Create,
	STATE_Move,
	STATE_Attack1,
	STATE_Transition,
	STATE_PHASE2Create,
	STATE_PHASE2,
	STATE_Attack2,
	STATE_Attack3,
	STATE_Return,
};

class TBossObj : public TNpcObj
{
public:
	void    HitOverlap(TObject* pObj, THitResult hRet) override;
	TMapObj* m_pMap = nullptr;
	void SetVertexData() override;
	virtual void Init() override;
	virtual void Frame() override;
	virtual void Render()override;
	virtual void Release()override;
public:
	BossState m_state = BossState::STATE_Create;
public:
	TVector2 m_vInitedPos;
	TVector2 m_vMapCenter = { 13440.0f , 450.0f };
	TVector2 m_vDir = { -1.0f, 0.0f };
	UINT	 m_iLimitedDis = 300.0f;
	float    m_ftrigger = 0.1f;
	float    m_fNextState = 3.0f;
	float    m_fCreateTime = 2.6f;
	float	 m_fCurrentTime = 0.0f;
	float    m_fTransTime = 3.0f;
	bool     m_bHealing = false;
	float    m_fPhase2CurrentTime = 3.0f;
	std::vector<TVector2> m_MissleDirList;
public:
	void GetState(bool state);
	bool m_bGameState;
public:
	INT m_iPreHP;
public:
	TBossObj(const TVector2& initialPos)
	{
		TNpcObj::m_HP= m_iPreHP = 200;
		m_fSpeed = 100.0f;
		m_vInitedPos = initialPos; // 초기 위치 설정
		m_vPos = initialPos;       // 현재 위치도 초기 위치로 설정
	}
public:
	TObjectType GetType() const override
	{
		return TObjectType::Boss;
	}
public:
	HeroView BossView = HeroView::LeftView;
};

