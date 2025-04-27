#pragma once
#include "Matrix.h"
#include "TInput.h"

class TCamera
{
public:
	Matrix		 m_matView;
	Matrix		 m_matProj;

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
	virtual Matrix Update(TVector4 vDirValue);
	Matrix UpdateVector();
	void CreateViewMatrix(
		TVector3 vCameraPos,    // ī�޶� ��ġ
		TVector3 vCameraTarget, // ī�޶� �ٶ󺸴� ��ġ
		TVector3 vCameraUp = {0,1,0});    // ī�޶��� ���� ����
	void CreateProjMatrix(
		float fFov,		        // �þ߰�(����)
		float fAspect,			// ȭ�����(����/����)
		float fNear,			// �ٴܸ� �Ÿ�
		float fFar);		    // ���ܸ� �Ÿ�;

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
	Matrix Update(TVector4 vDirValue) { return Matrix::Identity(); }
};