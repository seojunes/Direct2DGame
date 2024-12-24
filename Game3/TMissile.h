#pragma once
#include "THeroObj.h"
class TMissileObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;
	THeroObj* m_pHero = nullptr;
public:
	bool			m_bDead = false;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void SetHero(THeroObj* pHero) { m_pHero = pHero; }
	void SetDirrection(HeroView view);
	virtual void Frame() override;
	virtual void SetVertexData();

	TMissileObj()
	{
		m_fSpeed = 400.0f;
		m_vDir.x = 1.0f;
	}
};

