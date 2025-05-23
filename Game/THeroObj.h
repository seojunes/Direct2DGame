#pragma once
#include "TMapObj.h"
#include "TCollisionManager.h"
#include "TProjectile.h"
#include "TRadderObj.h"

//#include ""
//#include "Collision.h"

enum class HeroState     // 그냥 enum을 사용할시에는 전역으로 사용되어서 충돌 가능성이 있다. 그래서 enum class사용해주는것이 안정적이다.
{
	Idle = 0,
	RightRun,
	LeftRun,
	Jump,
	Radder,
	Shotting,
	Victory,
}; // 상태 정의


enum class BossRoomMovingState
{
	STATE_UNABLE = 0,
	STATE_ABLE,
};
enum HeroView
{
	RightView=0,
	LeftView,
};



class THeroObj : public Object2D
{	
	TMapObj* m_pMap = nullptr;
	TRadderObj* m_pRadder = nullptr;
	const int m_MaxJunp = 3;
// 스프라이트 조절단
public:
	std::shared_ptr<TProjectile>		m_pProjectile;
	std::vector<RECT> m_rtIdleFrames; // 정지 애니메이션 프레임 리스트
	std::vector<RECT> m_rtWalkFrames; // 걷기 애니메이션 프레임 리스트
	std::vector<RECT> m_rtJumpFrames; // 걷기 애니메이션 프레임 리스트
	std::vector<RECT> m_rtShotFrames; // 슈팅 애니메이션 프레임 리스트
	std::vector<RECT> m_rtShotNRunFrames; // 러닝슈팅 애니메이션 프레임 리스트
	std::vector<RECT> m_rtVictoryFrames; // 빅토리 애니메이션 프레임 리스트
	std::vector<RECT> m_rtRadderFrames; // 사다리 애니메이션 프레임 리스트
	std::vector<RECT> m_rtShotNJumpFrames; // 점프샷 애니메이션 프레임 리스트


	UINT m_iIdleFrame = 0;            // 현재 정지 애니메이션 프레임
	UINT m_iWalkFrame = 0;            // 현재 걷기 애니메이션 프레임
	UINT m_iJumpFrame = 0;			  // 현재 점프 애니메이션 프레임
	UINT m_iShotFrame = 0;			  // 현재 발사 애니메이션 프레임
	UINT m_iVictoryFrame = 0;
	UINT m_iRadderFrame = 0;


	float m_fCurrentTime = 0.0f;      // 현재 프레임 지속 시간
	float m_fWalkFrameTime = 0.2f;    // 한 프레임당 지속 시간
	float m_fJumpFrameTime = 0.2f;    // 한 프레임당 지속 시간
	float m_fIdleFrameTime = 0.1f;	  // idle 한 프레임당 지속시간	
	float m_fVictoryFrameTime = 0.3f;
	float m_fRadderFrameTime = 0.1f;

	float m_fShootingMotionTime = 0.0f; //슈팅모션 남은지속시간.
	float m_fMaxMotionTime = 0.2f;		//슈팅모션 최대지속시간.
	bool  m_bLoop = true;               // 반복 여부
// 추가상태변화	
public:
	bool m_bIsShooting = false;
	bool m_bInvincible = false;					// 무적 상태 여부
	float m_fInvincibleTime = 0.0f;				// 무적 상태 남은 시간
	const float m_fMaxInvincibleTime = 2.0f;	// 무적 상태 지속 시간
	bool m_bKeyinput = true;
public:
	void TakeDamage(int damage);
	bool m_bEnd = false;
	bool IsInvincible() const	{ return m_bInvincible; }
public:
	HeroState m_CurrentState = HeroState::Jump; // 초기 상태는 Jump
	HeroView m_CurrentView = HeroView::RightView;  // 초기 상태는 RightView
public:
	BossRoomMovingState m_BossMoving = BossRoomMovingState::STATE_UNABLE;

public:
	int m_iJumpingCount = 0;
	bool m_bIsJumping = true;
	float m_fJumpSpeed = 450.0f;
	float m_fVerticalSpeed = 0.0f;
	float m_fGravity = 980.0f;
	float m_fGroundY = 1800.0f;
public:
	float m_fChargingTime = 0.0f;
	bool m_bOnCharing = false;
	bool m_bCharging = false;
	TVector2 m_vRightDir = { 1.0f, 0.0f };
	TVector2 m_vLeftDir = { -1.0f, 0.0f };
public:
	INT m_HeroHP = 400;
	std::vector <SRect> spriteData;
	void SetData(vector<vector<RECT>> SpriteList);
public:
	void SetMap(TMapObj* pMap) { m_pMap = pMap; }
	void SetRadder(TRadderObj* pRadder) { m_pRadder = pRadder; }
	virtual void Init() override;
	virtual void Frame() override;
	virtual void Render()override;
	virtual void Release()override;
	virtual void SetVertexData()override;
	void HitOverlap(Object* pObj, HitResult hRes) override;
	
public:
	void GetGroundH(float height);
	
	
public:
	ObjectType GetType() const override 
	{
		return ObjectType::Hero;
	}
public:
	THeroObj()
	{
		m_fSpeed = 500.0f;
	}
public:
	bool m_bAttacked = false;
};

