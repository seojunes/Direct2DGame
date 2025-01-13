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
	Shotting,
}; // ���� ����


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



class THeroObj : public TObject2D
{	
	TMapObj* m_pMap = nullptr;
	const int m_MaxJunp = 3;


//public:
//	static THeroObj* GetInstance()
//	{
//		static THeroObj instance;
//		return &instance;
//	}


public:
	std::shared_ptr<TProjectile>		m_pProjectile;
	std::vector<RECT> m_rtIdleFrames; // ���� �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtWalkFrames; // �ȱ� �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtJumpFrames; // �ȱ� �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtShotFrames; // ���� �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtShotNRunFrames; // ���� �ִϸ��̼� ������ ����Ʈ


	UINT m_iIdleFrame = 0;            // ���� ���� �ִϸ��̼� ������
	UINT m_iWalkFrame = 0;            // ���� �ȱ� �ִϸ��̼� ������
	UINT m_iJumpFrame = 0;			  // ���� ���� �ִϸ��̼� ������
	UINT m_iShotFrame = 0;			  // ���� �߻� �ִϸ��̼� ������


	float m_fCurrentTime = 0.0f;      // ���� ������ ���� �ð�
	float m_fWalkFrameTime = 0.2f;    // �� �����Ӵ� ���� �ð�
	float m_fJumpFrameTime = 0.1f;    // �� �����Ӵ� ���� �ð�
	float m_fIdleFrameTime = 0.1f;	  // idle �� �����Ӵ� ���ӽð�	

	float m_fShootingMotionTime = 0.0f; //���ø�� �������ӽð�.
	float m_fMaxMotionTime = 1.0f;		//���ø�� �ִ����ӽð�.
	bool  m_bLoop = true;               // �ݺ� ����
	
public:
	bool m_bIsShooting = false;
	bool m_bInvincible = false;					// ���� ���� ����
	float m_fInvincibleTime = 0.0f;				// ���� ���� ���� �ð�
	const float m_fMaxInvincibleTime = 2.0f;	// ���� ���� ���� �ð�
public:
	void TakeDamage(int damage);
	bool IsInvincible() const	{ return m_bInvincible; }
public:
	HeroState m_CurrentState = HeroState::Jump; // �ʱ� ���´� Jump
	HeroView m_CurrentView = HeroView::RightView;  // �ʱ� ���´� RightView
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
	INT m_HP = 100;
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
		m_fSpeed = 500.0f;
	}
};

