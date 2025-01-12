#include "TControlGUI.h"
#include "TMapObj.h"
#include "THeroObj.h"
#pragma once
class TUiHpBar :public TImageGUI
{
	TMapObj* m_pMap = nullptr;
	THeroObj* m_pHero = nullptr;
public:
	void SetHero(THeroObj* pHero) { m_pHero = pHero; }
public:
	UINT m_iHP;
	UINT m_iMaxHp;
	float m_fHpRatio = 1.0f;
	TVector2 m_vInitialScale; // 초기 크기 저장

public:
	void Frame() override;
	//void Render() override;
public:
	TUiHpBar(UINT hp)
	{
		m_iHP = m_iMaxHp = hp;
	}
};

