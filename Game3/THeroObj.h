#pragma once
#include "TMapObj.h"


enum class HeroState     // �׳� enum�� ����ҽÿ��� �������� ���Ǿ �浹 ���ɼ��� �ִ�. �׷��� enum class������ִ°��� �������̴�.
{
	RightRun = 0,
	LeftRun,
	Jump,
}; // ���� ����

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
	std::vector<RECT> m_rtWalkFrames; // �ȱ� �ִϸ��̼� ������ ����Ʈ
	std::vector<RECT> m_rtJumpFrames; // �ȱ� �ִϸ��̼� ������ ����Ʈ
	UINT m_iWalkFrame = 0;            // ���� �ȱ� �ִϸ��̼� ������
	UINT m_iJumpFrame = 0;			  // ���� ���� �ִϸ��̼� ������	
	float m_fCurrentTime = 0.0f;      // ���� ������ ���� �ð�
	float m_fFrameTime = 0.1f;        // �� �����Ӵ� ���� �ð�
	bool m_bLoop = true;              // �ݺ� ����

public:
	HeroState m_CurrentState = HeroState::RightRun; // �ʱ� ���´� RightRun
	HeroView m_CurrentView = HeroView::RightView;  // �ʱ� ���´� RightView

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

