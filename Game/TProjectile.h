#pragma once
#include "TEffectObj.h"

enum Shooter
{
	OWNER_HERO = 0,
	OWNER_MON2,
	OWNER_MON3,
	OWNER_BOSS1,
	OWNER_BOSS2,
	OWNER_BOSS3,
};

class TProjectileEffect : public TEffectObj
{
public:
	TVector2 m_vObjPos;
	TVector2 m_vOffsetPos;
	TVector2 m_vPrePos;
	float    m_fTimer = 0.0f;
	Shooter m_pOwnerType = Shooter::OWNER_HERO;
	float temp = 0.0f;

	void Frame();
	void Render();
	void    HitOverlap(TObject* pObj, THitResult hRet) override;
	TObjectType GetType() const override
	{
		return TObjectType::Projectile;
	}
};

class TProjectile : public TObject2D
{
	TWorld* m_pWorld = nullptr;

public:
	bool  m_bOncharging = false;
public:
	using tObject = std::shared_ptr<TProjectileEffect>;
	std::list< tObject>   m_datalist;
	void   AddEffect(TVector2 vStart, TVector2 tEnd, TVector2 direction, Shooter owner, TObject* m_pOwner, bool m_bOncharging = false);
	
	//void   ApplyOwnerType(const TObjectType& InObjectType);
	void   Init();
	void   Frame(TVector2 vPos);
	void   Render(TVector2 vCamera);
	void   Release();
	INT	 m_iDamage = 10;

public:
	//TProjectile();
	TProjectile(TWorld* pWorld);

	TObjectType GetType() const override
	{
		return TObjectType::Projectile;
	}
	/*TObjectType GetOwnerType() const { return OwnerType; }
protected:
	TObjectType OwnerType = TObjectType::None;*/
};

