#pragma once
#include "TMapObj.h"
class TRadderObj : public Object2D
{
	TMapObj* m_pMap = nullptr;

public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void HitOverlap(Object* pObj, THitResult hRes) override;
	ObjectType GetType() const override
	{
		return ObjectType::Radder;
	}
};