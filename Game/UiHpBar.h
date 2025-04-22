#include "ControlGUI.h"
#include "TMapObj.h"
#include "THeroObj.h"
#include "TBossObj.h"
#pragma once

enum HPBAR_OWNER
{
	BAROWNER_HERO = 0,
	BAROWNER_BOSS,
};

class UiHpBar :public ImageGUI
{
	THeroObj* m_pHero = nullptr;
	TBossObj* m_pBoss = nullptr;
public:
	void SetHero(THeroObj* pHero) { m_pHero = pHero; }
	void SetBoss(TBossObj* pBoss) { m_pBoss = pBoss; }
	void SetHeroHp(INT hp);
	void SetBossHp(INT hp);
public:
	INT m_iHP;
	INT m_iMaxHp;
	float m_fHpRatio = 1.0f;
	TVector2 m_vInitialScale; // 초기 크기 저장
public:
	INT m_iBHP;
	INT m_iBMaxHp;
	float m_fBHpRatio = 1.0f;
	TVector2 m_vBInitialScale; // 초기 크기 저장
public:
	HPBAR_OWNER m_eBarOwner = HPBAR_OWNER::BAROWNER_HERO;
public:
	void Frame() override;
	//void Render() override;
public:
	UiHpBar(HPBAR_OWNER owner)
	{
		m_eBarOwner = owner;
	}
};

class UIHpTile : public ImageGUI
{
	
};

