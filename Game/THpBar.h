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
	float m_fShowTime = 0.0f; // HP 바 표시 타이머
	const float m_fMaxShowTime = 1.5f; // 표시 시간 (0.5초)
	float m_fDisplayedHpRatio = 1.0f; // 화면에 표시되는 HP 비율
	float m_fSmoothSpeed = 0.1f;    // 스무스하게 줄어드는 속도
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void SetNpc(TNpcObj* pNpc) { m_pNpc = pNpc; }
public:
	UINT m_iPreviousHP;
	UINT m_iHP;
	UINT m_iMaxHp;
	float m_fHpRatio = 1.0f;
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

