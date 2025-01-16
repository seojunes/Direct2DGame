#pragma once
#include "TObject2D.h"
enum class DropState
{
	STATE_DROP = 0,
	STATE_EXPLOSION,
	STATE_DEAD,
};

class TDropObj : public TObject2D
{
public:
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	void Frame() override;
	void SetData(vector<vector<RECT>> SpriteList);
	virtual void SetVertexData() override;
public:
	std::vector<RECT> m_rtDropFrame;
	std::vector<RECT> m_rtExplosion;
	UINT m_iDropFrame = 0;
	UINT m_iExplosion = 0;
	float m_fDropFrameTime = 0.2f;
	float m_fExplosionFrameTime = 0.1f;

	float m_fCurrentTime = 0.0f;
	float m_fExplosionTime = 0.5f;
	DropState m_state = DropState::STATE_DROP;
	bool  m_bExplosion = false;
};

