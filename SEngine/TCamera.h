#pragma once
#include "TMatrix.h"
#include "TInput.h"

class TCamera
{
public:
	TMatrix		 m_matView;
	TMatrix		 m_matProj;

	TVector3	 m_vPosition;
	TVector3	 m_vTarget;

	float		 m_fRadius;
	TQuaternion  m_qRotation;
	
	TVector3 m_vLook;
	TVector3 m_vUp;
	TVector3 m_vRight;

	float	 m_fSpeed = 10.0f;

	float    m_fYaw = 0.0f;
	float    m_fPitch = 0.0f;
	float    m_fRoll = 0.0f;
public:
	virtual void Tick();
	virtual TMatrix Update(TVector4 vDirValue);
	TMatrix UpdateVector();
	void CreateViewMatrix(
		TVector3 vCameraPos,    // 카메라 위치
		TVector3 vCameraTarget, // 카메라가 바라보는 위치
		TVector3 vCameraUp = {0,1,0});    // 카메라의 위쪽 방향
	void CreateProjMatrix(
		float fFov,		        // 시야각(수평)
		float fAspect,			// 화면비율(가로/세로)
		float fNear,			// 근단면 거리
		float fFar);		    // 원단면 거리;

public:
	void MoveLook( );
	void BackLook();
	void MoveUp( );
	void BackUp();
	void MoveRight( );
	void BackRight();
	float m_fMoveSpeed =1.0f;
};

class SBackViewCamera : public TCamera
{
public:
	void Tick();
	TMatrix Update(TVector4 vDirValue) { return TMatrix::Identity(); }
};