#pragma once
#include "TObject2D.h"
class TItem :public TObject2D
{
public:
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	void Frame() override;
	void SetData(vector<vector<RECT>> SpriteList);
	virtual void SetVertexData() override;

	void setMonPos(TVector2 pos);

	TVector2 m_vResponPos;

public: 
	/*TItem(TVector2 pos)
	{
		m_vResponPos = pos;
	}*/

public:
	std::vector<RECT> m_rtItemFrames; // 엑티브포탈 애니메이션 프레임 리스트
	UINT m_iItemFrame = 0;            // 현재 정지 애니메이션 프레임

	float m_fItemFrameTime = 0.5f;
	float m_fCurrentTime = 0.0f;
};

