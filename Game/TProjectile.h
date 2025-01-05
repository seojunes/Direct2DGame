#pragma once
#include "TEffectObj.h"
class TProjectileEffect : public TEffectObj
{
public:
	TVector2 m_vHeroPos;
	TVector2 m_vOffsetPos;
	TVector2 m_vPrePos;
	float    m_fTimer = 0.0f;
	void Frame();
	void Render();
};
class TProjectile
{
	TWorld* m_pWorld = nullptr;
public:
	using tObject = std::shared_ptr<TProjectileEffect>;
	std::list< tObject>   m_datalist;
	void   AddEffect(TVector2 vStart, TVector2 tEnd);
	void   Init();
	void   Frame(TVector2 vPos);
	void   Render(TVector2 vCamera);
	void   Release();
public:
	//TProjectile();
	TProjectile(TWorld* pWorld);
};

