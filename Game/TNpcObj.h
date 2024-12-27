#pragma once
#include "TMapObj.h"
class TNpcObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	virtual void Frame() override;
	virtual void SetVertexData();
	TNpcObj()
	{
		m_fSpeed = 150.0f;
		m_vDir.x = 1.0f;
		m_vDir.y = 1.0f;
	}
};

