#pragma once
#include "TObject2D.h"
class TBossCreate : public TObject2D
{
public:
	void GetState(bool state);
	void Frame() override;
	void SetVertexData() override;
	void HitOverlap(TObject* pObj, THitResult hRes) override;
public:
	bool m_bGameState;
	bool m_bCreate = false;
};

