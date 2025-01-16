#pragma once
#include "TObject2D.h"
class TDropObj : public TObject2D
{
public:
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	void Frame() override;
};

