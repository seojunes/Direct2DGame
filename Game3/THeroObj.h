#pragma once
#include "TObject2D.h"
class THeroObj : public TObject2D
{
private:

	int m_JumpCount = 0;
	const int m_MaxJunp = 3;
public:

	std::vector <SRect> spriteData;
	bool m_bIsJumping = false;
	float m_fJumpSpeed = 400.0f;
	float m_fVerticalSpeed = 0.0f;
	float m_fGravity = 980.0f;
	float m_fGroundY = 625.0f;
public:
	virtual void Frame() override;
	virtual void SetVertexData();
};

