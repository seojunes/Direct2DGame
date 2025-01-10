#pragma once
#include "TMapObj.h"

class TPortal : public TObject2D
{
	TMapObj* m_pMap = nullptr;
public:
	void SetVertexData() override;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	TObjectType GetType() const override
	{
		return TObjectType::Portal;
	}
};

