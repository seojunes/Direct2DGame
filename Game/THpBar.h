#pragma once
#include "TMapObj.h"
#include "TNpcObj.h"

enum HPSTATE
{
	STATE_IDEL = 0,
	STATE_CHANGE,
};

class  THpBar : public TObject2D
{
	TMapObj* m_pMap = nullptr;
	TNpcObj* m_pNpc = nullptr;
public:
	float m_fShowTime = 0.5f;
	float m_fcurrentTime = 0.0f;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void SetNpc(TNpcObj* pNpc) { m_pNpc = pNpc; }
	UINT m_iHP;
	UINT m_iMaxHp;
	float m_fHpRatio = (float)m_iHP / m_iMaxHp;
	TVector2 m_vInitialScale; // 초기 크기 저장
public:
	HPSTATE m_HPSTATE = HPSTATE::STATE_IDEL;
public:
	void Frame() override;
	void Render() override;
public:
	THpBar(UINT hp)
	{
		m_iHP = m_iMaxHp = hp;
	}
};

