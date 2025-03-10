#pragma once
#include "TMatrix.h"

class TCamera
{
public:
	TMatrix m_matView;
	TMatrix m_matProj;

	TVector3 m_vPosition;
	TVector3 m_vTarget;

	float    m_fRadius;
	TQuaternion m_qRotation;
	
	TVector3 m_vLook;
	TVector3 m_vUp;
	TVector3 m_vRight;

	float    m_fYaw = 0.0f;
	float    m_fPitch = 0.0f;
	float    m_fRoll = 0.0f;
public:
	TMatrix Update(TVector4 vDirValue);
	TMatrix UpdateVector();
	void CreateViewMatrix(
		TVector3 vCameraPos, // ī�޶� ��ġ
		TVector3 vCameraTarget, // ī�޶� �ٶ󺸴� ��ġ
		TVector3 vCameraUp); // ī�޶��� ���� ����
	void CreateProjMatrix(
		float fFov,		// �þ߰�(����)
		float fAspect,  // ȭ�����(����/����)
		float fNear,    // �ٴܸ� �Ÿ�
		float fFar);		// ���ܸ� �Ÿ�;
};