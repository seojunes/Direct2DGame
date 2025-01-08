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
	void    HitOverlap(TObject* pObj, THitResult hRet) override;
};
class TProjectile : public TObject2D
{
	TWorld* m_pWorld = nullptr;
public:
	bool  m_bOncharging = false;
public:
	using tObject = std::shared_ptr<TProjectileEffect>;
	std::list< tObject>   m_datalist;
	void   AddEffect(TVector2 vStart, TVector2 tEnd, UINT direction, bool  m_bOncharging);
	void   Init();
	void   Frame(TVector2 vPos);
	void   Render(TVector2 vCamera);
	void   Release();
	UINT	 m_iDamage = 10;
	
public:
	TObjectType GetType() const override 
	{
		return TObjectType::Projectile;
	}
public:
	//TProjectile();
	TProjectile(TWorld* pWorld);
};

