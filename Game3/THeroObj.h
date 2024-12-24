#pragma once
#include "TMapObj.h"


enum class HeroState     // 그냥 enum을 사용할시에는 전역으로 사용되어서 충돌 가능성이 있다. 그래서 enum class사용해주는것이 안정적이다.
{
	RightRun = 0,
	LeftRun,
	Jump,
}; // 상태 정의

enum HeroView
{
	RightView=0,
	LeftView,
};


class THeroObj : public TObject2D
{
	TMapObj* m_pMap = nullptr;
private:
	const int m_MaxJunp = 3;
public:
	std::vector<RECT> m_rtWalkFrames; // 걷기 애니메이션 프레임 리스트
	std::vector<RECT> m_rtJumpFrames; // 걷기 애니메이션 프레임 리스트
	UINT m_iWalkFrame = 0;            // 현재 걷기 애니메이션 프레임
	UINT m_iJumpFrame = 0;			  // 현재 점프 애니메이션 프레임	
	float m_fCurrentTime = 0.0f;      // 현재 프레임 지속 시간
	float m_fFrameTime = 0.1f;        // 한 프레임당 지속 시간
	bool m_bLoop = true;              // 반복 여부

public:
	HeroState m_CurrentState = HeroState::RightRun; // 초기 상태는 RightRun
	HeroView m_CurrentView = HeroView::RightView;  // 초기 상태는 RightView

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
	void SetData(vector<vector<RECT>> SpriteList);
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void InitWalkAnimation();
	virtual void Frame() override;
	virtual void SetVertexData();
	THeroObj()
	{
		m_fSpeed = 300.0f;
	}
};

