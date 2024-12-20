#pragma once
#include "TMapObj.h"

class THeroObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;
private:
	const int m_MaxJunp = 3;
public:
	enum HeroState { RightRun, LeftRun }; // 상태 정의
	HeroState m_CurrentState = RightRun; // 초기 상태는 RightRun
public:
	int m_iJumpingCount = 0;
	bool m_bIsJumping = false;
	float m_fJumpSpeed = 400.0f;
	float m_fVerticalSpeed = 0.0f;
	float m_fGravity = 980.0f;
	float m_fGroundY = 595.0f;
public:
	std::vector <SRect> spriteData;
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	virtual void Frame() override;
	virtual void SetVertexData();
	THeroObj()
	{
		m_fSpeed = 300.0f;
	}
};

