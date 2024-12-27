#pragma once
#include "TGameCore.h"
#include "TMapObj.h"
#include "THeroObj.h"
#include "TEffectObj.h"
#include "TSound.h"
#include "TNpcObj.h"
#include "TMissile.h"
#include "HPBar.h"
using RECT_ARRAY = std::vector<RECT>;

class Sample : public TGameCore
{
	TVector2        m_vCamera = { 640.0f, 400.0f };
	TTexture*		m_pBitmap1Mask = nullptr;
	using tObject = std::shared_ptr<TObject>;

	std::shared_ptr<TMapObj>	m_pMap = nullptr;
	std::shared_ptr<THeroObj>	m_pHero = nullptr;
	std::vector<tObject>		m_NpcList;
	std::list<tObject>			m_EffectList;
	std::vector<tObject>		m_MissileList;
	UINT						m_Npccount = 300;

	TSound* m_pSound = nullptr;
	TSound* m_pJumpSound = nullptr;
	TSound* m_pShotSound = nullptr;
	TSound* m_pCrashSound = nullptr;
	std::vector<RECT_ARRAY>  m_rtSpriteList;
	std::vector<T_STR_VECTOR> m_szSpriteList;
public:
	void   TransPivot();
	void   AddEffect(TVector2 tStart, TVector2 tEnd);
	void   AddMissile(TVector2 tStart, TVector2 tEnd);
	void   AddEffectSingle(TVector2 tStart, TVector2 tEnd);
	bool GameDataLoad(W_STR filename);
	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
public:
	bool CreateMap();
	bool CreateHero();
	bool CreateNPC();
	bool CreateEffect();
	
};

