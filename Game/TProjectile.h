#pragma once
#include "EffectObj.h"

enum Shooter
{
	OWNER_HERO = 0,
	OWNER_MON2,
	OWNER_MON3,
	OWNER_BOSS1,
	OWNER_BOSS2,
	OWNER_BOSS3,
};

class TProjectileEffect : public EffectObj
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
	void    HitOverlap(Object* pObj, HitResult hRet) override;
	ObjectType GetType() const override
	{
		return ObjectType::Projectile;
	}
};

class TProjectile : public Object2D
{
	TWorld* m_pWorld = nullptr;

public:
	bool  m_bOncharging = false;
public:
	using tObject = std::shared_ptr<TProjectileEffect>;
	std::list< tObject>   m_datalist;
	void   AddEffect(TVector2 vStart, TVector2 tEnd, TVector2 direction, Shooter owner, Object* m_pOwner, bool m_bOncharging = false);
	
	//void   ApplyOwnerType(const ObjectType& InObjectType);
	void   Init();
	void   Frame(TVector2 vPos);
	//void   Frame() override;
	void   Render(TVector2 vCamera);
	void   Release();
	INT	 m_iDamage = 10;
	

public:
	//TProjectile();
	TProjectile(TWorld* pWorld);

	ObjectType GetType() const override
	{
		return ObjectType::Projectile;
	}
	/*ObjectType GetOwnerType() const { return OwnerType; }
protected:
	ObjectType OwnerType = ObjectType::None;*/
};

