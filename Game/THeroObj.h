#pragma once
#include "TMapObj.h"
#include "TCollisionManager.h"
#include "TProjectile.h"

//#include ""
//#include "TCollision.h"

enum class HeroState     // 그냥 enum을 사용할시에는 전역으로 사용되어서 충돌 가능성이 있다. 그래서 enum class사용해주는것이 안정적이다.
{
	Idle = 0,
	RightRun,
	LeftRun,
	Jump,
	Shoting,
}; // 상태 정의

enum HeroView
{
	RightView=0,
	LeftView,
};


class THeroObj : public TObject2D
{	
	TMapObj* m_pMap = nullptr;
	const int m_MaxJunp = 3;
public:
	std::shared_ptr<TProjectile>		m_pProjectile;
	std::vector<RECT> m_rtWalkFrames; // 걷기 애니메이션 프레임 리스트
	std::vector<RECT> m_rtJumpFrames; // 걷기 애니메이션 프레임 리스트
	UINT m_iWalkFrame = 0;            // 현재 걷기 애니메이션 프레임
	UINT m_iJumpFrame = 0;			  // 현재 점프 애니메이션 프레임	
	float m_fCurrentTime = 0.0f;      // 현재 프레임 지속 시간
	float m_fFrameTime = 0.1f;        // 한 프레임당 지속 시간
	bool m_bLoop = true;              // 반복 여부
	
public:
	//void InitHero(UINT maxHP, float hpBarWidth, float hpBarHeight);
	//void TakeDamage(UINT damage);
	//void Heal(UINT healAmount);
public:
	HeroState m_CurrentState = HeroState::Jump; // 초기 상태는 RightRun
	HeroView m_CurrentView = HeroView::RightView;  // 초기 상태는 RightView
	

public:
	int m_iJumpingCount = 0;
	bool m_bIsJumping = true;
	float m_fJumpSpeed = 400.0f;
	float m_fVerticalSpeed = 0.0f;
	float m_fGravity = 980.0f;
	float m_fGroundY = 1800.0f;
public:
	float m_fChargingTime = 0.0f;
	bool m_bOnCharing = false;
	bool m_bCharging = false;
public:
	UINT m_HP = 100;
	std::vector <SRect> spriteData;
	void SetData(vector<vector<RECT>> SpriteList);
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	virtual void Init() override;
	virtual void Frame() override;
	virtual void Render()override;
	virtual void Release()override;
	virtual void SetVertexData()override;
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	
public:
	void GetGroundH(float height);
	
	
public:
	TObjectType GetType() const override 
	{
		return TObjectType::Hero;
	}
public:
	THeroObj()
	{
		m_fSpeed = 300.0f;
	}
};

