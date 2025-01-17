#pragma once
#include "TObject2D.h"
class TItem :public TObject2D
{
public:
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	void Frame() override;
	void SetData(vector<vector<RECT>> SpriteList);
	virtual void SetVertexData() override;
};

