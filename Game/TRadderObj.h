#pragma once
#include "TMapObj.h"
class TRadderObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;

public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	TObjectType GetType() const override
	{
		return TObjectType::Radder;
	}
};