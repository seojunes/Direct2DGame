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
	std::vector<RECT> m_rtPortalFrames; // �����Ż �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtAPortalFrames; // ��Ƽ����Ż �ִϸ��̼� ������ ����Ʈ
	UINT m_iPortalFrame = 0;            // ���� ���� �ִϸ��̼� ������
	UINT m_iAPortalFrame = 0;            // ���� ���� �ִϸ��̼� ������

	float m_fPortalFrameTime = 0.1f;
	
	bool m_bLoop = true;
	bool m_bAble = false;
	void SetData(vector<vector<RECT>> SpriteList);
public:
	PortalActiveState m_eActive = PortalActiveState::STATE_INACTIVE;
};

