#pragma once
#include "TNpcObj.h"

enum class BossState
{
	STATE_Idle = 0,
	STATE_Create,
	STATE_Move,
	STATE_Attack1,
	STATE_Attack2,
	STATE_Attack3,
	STATE_Return,
};

class TBossObj : public TNpcObj
{
public:
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
	UINT	 m_iLimitedDis = 300.0f;
	float    m_ftrigger = 0.1f;
	float    m_fNextState = 3.0f;
	float    m_fCreateTime = 3.0f;
	float	 m_fCurrentTime = 0.0f;
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
		m_vInitedPos = initialPos; // �ʱ� ��ġ ����
		m_vPos = initialPos;       // ���� ��ġ�� �ʱ� ��ġ�� ����
	}
public:
	TObjectType GetType() const override
	{
		return TObjectType::Boss;
	}
};

