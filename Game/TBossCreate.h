#pragma once
#include "Object2D.h"
class TBossCreate : public Object2D
{
public:
	void GetState(bool state);
	void Frame() override;
	void SetVertexData() override;
	void HitOverlap(Object* pObj, THitResult hRes) override;
public:
	bool m_bGameState;
	bool m_bCreate = false;
};

