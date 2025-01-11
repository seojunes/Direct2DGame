#pragma once
#include "TNpcObj.h"

enum class BossState
{
	STATE_Idle = 0,
	STATE_Create,
	STATE_Move,
	STATE_Attack,
	STATE_Return,
};

class TBossObj : public TNpcObj
{
public:
	TMapObj* m_pMap = nullptr;
	void SetVertexData() override;
	//virtual void Init() override;
	virtual void Frame() override;
	//virtual void Render()override;
	//virtual void Release()override;
public:
	BossState m_state = BossState::STATE_Create;
public:
	TVector2 m_vInitedPos;
	UINT	 m_iLimitedDis = 300.0f;
	float    m_ftrigger = 1.0f;
public:
	void GetState(bool state);
	bool m_bGameState;
public:
	TBossObj(const TVector2& initialPos)
	{
		TNpcObj::m_HP = 10;
		m_fSpeed = 100.0f;
		m_vInitedPos = initialPos; // 초기 위치 설정
		m_vPos = initialPos;       // 현재 위치도 초기 위치로 설정
	}
};

