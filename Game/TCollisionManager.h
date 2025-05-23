#pragma once
#include "TMapObj.h"

class  TCollisionManager : public Object2D
{
	TMapObj* m_pMap = nullptr;
public:
	std::vector<TVector2> rectSList;
	std::vector<TVector2> rectEList;
	TVector2 m_vStart;
	TVector2 m_vEnd;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	//virtual void SetVertexData();
	void HitOverlap(Object* pObj, HitResult hRes) override;
	//vector<TVector2> MakeSBox();
	//vector<TVector2> MakeEBox();
	ObjectType GetType() const override
	{
		return ObjectType::Wall;
	}
};

