#pragma once
#include "TMapObj.h"
class TVictory :public Object2D
{
	TMapObj* m_pMap = nullptr;
	
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void Frame() override;
public:
	TVictory()
	{
		m_vDir = { 0.0f , 1.0f };
		m_fSpeed = 150.0f;
	}
};

