#pragma once
#include "TMapObj.h"
#include "TCollisionManager.h"
#include "TProjectile.h"

//#include ""
//#include "TCollision.h"

enum class HeroState     // �׳� enum�� ����ҽÿ��� �������� ���Ǿ �浹 ���ɼ��� �ִ�. �׷��� enum class������ִ°��� �������̴�.
{
	Idle = 0,
	RightRun,
	LeftRun,
	Jump,
	Shoting,
}; // ���� ����

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
	std::vector<RECT> m_rtWalkFrames; // �ȱ� �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtJumpFrames; // �ȱ� �ִϸ��̼� ������ ����Ʈ
	UINT m_iWalkFrame = 0;            // ���� �ȱ� �ִϸ��̼� ������
	UINT m_iJumpFrame = 0;			  // ���� ���� �ִϸ��̼� ������	
	float m_fCurrentTime = 0.0f;      // ���� ������ ���� �ð�
	float m_fFrameTime = 0.1f;        // �� �����Ӵ� ���� �ð�
	bool m_bLoop = true;              // �ݺ� ����
	
public:
	//void InitHero(UINT maxHP, float hpBarWidth, float hpBarHeight);
	//void TakeDamage(UINT damage);
	//void Heal(UINT healAmount);
public:
	HeroState m_CurrentState = HeroState::Jump; // �ʱ� ���´� RightRun
	HeroView m_CurrentView = HeroView::RightView;  // �ʱ� ���´� RightView
	

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

