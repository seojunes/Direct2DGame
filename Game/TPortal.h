#pragma once
#include "TMapObj.h"

enum class PortalActiveState
{
	STATE_ACTIVE = 0,
	STATE_INACTIVE,
};


class TPortal : public Object2D
{
	TMapObj* m_pMap = nullptr;
public:
	void SetVertexData() override;
	float m_fCurrentTime = 0.0f;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void HitOverlap(Object* pObj, HitResult hRes) override;
	ObjectType GetType() const override
	{
		return ObjectType::Portal;
	}
	void Frame() override;
public:
	std::vector<RECT> m_rtPortalFrames; // 노멀포탈 애니메이션 프레임 리스트
	std::vector<RECT> m_rtAPortalFrames; // 엑티브포탈 애니메이션 프레임 리스트
	UINT m_iPortalFrame = 0;            // 현재 정지 애니메이션 프레임
	UINT m_iAPortalFrame = 0;            // 현재 정지 애니메이션 프레임

	float m_fPortalFrameTime = 0.1f;
	
	bool m_bLoop = true;
	bool m_bAble = false;
	void SetData(vector<vector<RECT>> SpriteList);
public:
	PortalActiveState m_eActive = PortalActiveState::STATE_INACTIVE;
};

