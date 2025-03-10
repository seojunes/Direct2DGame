#include "TCamera.h"
#include "TMath.h"
	void TCamera::CreateViewMatrix(TVector3 vPosition,
	TVector3 vTarget,
	TVector3 vUp)
{
	m_vPosition = vPosition;
	m_vTarget = vTarget;
	m_vUp = vUp;

	TMatrix m;
	TVector3 vDirection = vTarget - vPosition;
	vDirection = vDirection.Normal();
	float fDot = vUp | vDirection; // 내적
	TVector3 vC = vDirection * fDot;
	TVector3 vUpVector = vUp - (vDirection * fDot);
	vUpVector = vUpVector.Normal();
	TVector3 vRightVector = vUpVector ^ vDirection;

	m._11 = vRightVector.x;	m._12 = vUpVector.x;		m._13 = vDirection.x;
	m._21 = vRightVector.y;	m._22 = vUpVector.y;		m._23 = vDirection.y;
	m._31 = vRightVector.z;	m._32 = vUpVector.z;		m._33 = vDirection.z;

	m._41 = -(vPosition.x * m._11 + vPosition.y * m._21 + vPosition.z * m._31);
	m._42 = -(vPosition.x * m._12 + vPosition.y * m._22 + vPosition.z * m._32);
	m._43 = -(vPosition.x * m._13 + vPosition.y * m._23 + vPosition.z * m._33);
	m_matView = m;
}
// 원근투영,  직교투영	
void TCamera::CreateProjMatrix(
	float fFov,		// 시야각(수평)
	float fAspect,  // 화면비율(가로/세로)
	float fNearPlane,    // 근단면 거리
	float fFarPlane)		// 원단면 거리
{
	float m_fFov = fFov;
	float m_fAspect = fAspect;
	float m_fNearPlane = fNearPlane;
	float m_fFarPlane = fFarPlane;

	float    h, w, Q;

	h = 1 / tan(fFov * 0.5f);
	w = h / fAspect;
	Q = fFarPlane / (fFarPlane - fNearPlane);

	TMatrix ret;
	ZeroMemory(&ret, sizeof(TMatrix));
	ret._11 = w;
	ret._22 = h;
	ret._33 = Q;
	ret._43 = -Q * fNearPlane;
	ret._34 = 1;
	m_matProj = ret;
}
TMatrix TCamera::Update(TVector4 vDirValue)
{		
	TBASIS_EX::TQuaternion* pqRotation = (TBASIS_EX::TQuaternion*)&m_qRotation;
	TBASIS_EX::TVector3* pPosition = (TBASIS_EX::TVector3*)&m_vPosition;
	TBASIS_EX::TMatrix* pMatView = (TBASIS_EX::TMatrix*)&m_matView;

	m_fYaw += vDirValue.y;
	m_fPitch += vDirValue.x;
	m_fRoll += vDirValue.z;

	TBASIS_EX::D3DXQuaternionRotationYawPitchRoll(
		pqRotation,
		m_fYaw,
		m_fPitch,
		m_fRoll);

	m_vPosition += m_vLook * vDirValue.w;
	m_fRadius += vDirValue.w;

	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::D3DXMatrixAffineTransformation(
		&matRotation, 1.0f, NULL, 
		pqRotation, pPosition);
	TBASIS_EX::D3DXMatrixInverse(
		pMatView, NULL, &matRotation);

	return UpdateVector();
}

TMatrix TCamera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.Normalize();
	m_vRight.Normalize();
	m_vUp.Normalize();
	//D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);
	return m_matView;
}