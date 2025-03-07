#include "TMatrix.h"

TMatrix3 TMatrix3::operator *(const TMatrix3& mat)
{
	TMatrix3 ret;
	// 11  12  13      11  12  13
	// 21  22  23   *  11  12  13
	// 31  32  33      11  12  13   
	for (int iColumn = 0; iColumn < 3; iColumn++)
	{
		for (int iRow = 0; iRow < 3; iRow++)
		{
			ret.m[iRow][iColumn] =
				m[iRow][0] * mat.m[0][iColumn] +
				m[iRow][1] * mat.m[1][iColumn] +
				m[iRow][2] * mat.m[2][iColumn];
		}
	}
	return ret;
}
TMatrix3 TMatrix3::Transpose()
{
	TMatrix3 ret;
	ret._11 = _11; ret._12 = _21; ret._13 = _31;
	ret._21 = _12; ret._22 = _22; ret._23 = _32;
	ret._31 = _13; ret._32 = _23; ret._33 = _33;
	return ret;
}
void TMatrix3::Scale(float x, float y)
{
	_11 = x; _22 = y;
}
void TMatrix3::Scale(TVector2 s)
{
	_11 = s.x; _22 = s.y;
}
void TMatrix3::Rotate(float fRadian)
{
	float fCosTheta = cosf(fRadian);
	float fSinTheta = sinf(fRadian);
	_11 = fCosTheta;  _12 = fSinTheta;
	_21 = -fSinTheta; _22 = fCosTheta;
}
void TMatrix3::Trans(float x, float y)
{
	_31 = x; _32 = y;
}
void TMatrix3::Trans(TVector2 t)
{
	_31 = t.x; _32 = t.y;
}
void TMatrix3::SetIdentity() // 단위행렬
{
	_12 = _13 = 0.0f;
	_21 = _23 = 0.0f;
	_31 = _32 = 0.0f;
	_11 = _22 = _33 = 1.0f;
}
TMatrix3::TMatrix3(TVector2 row1, TVector2 row2, TVector2 row3)
{
	_11 = row1.x; _12 = row1.y; _13 = 0.0f;
	_21 = row2.x; _22 = row2.y; _23 = 0.0f;
	_31 = row3.x; _32 = row3.y; _33 = 1.0f;
}
TMatrix3::TMatrix3()
{
	SetIdentity();
}

/// <summary>
/// 
/// </summary>
/// <param name="mat"></param>
/// <returns></returns>
TMatrix TMatrix::operator *(const TMatrix& mat)
{
	TMatrix ret;
	for (int iColumn = 0; iColumn < 4; iColumn++)
	{
		for (int iRow = 0; iRow < 4; iRow++)
		{
			ret.m[iRow][iColumn] =
				m[iRow][0] * mat.m[0][iColumn] +
				m[iRow][1] * mat.m[1][iColumn] +
				m[iRow][2] * mat.m[2][iColumn] +
				m[iRow][3] * mat.m[3][iColumn];
		}
	}
	return ret;
}
TMatrix TMatrix::Transpose()
{
	TMatrix ret;
	ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
	ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
	ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
	ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;
	return ret;
}
void TMatrix::Scale(float x, float y, float z)
{
	_11 = x; _22 = y; _33 = z;
}
void TMatrix::Scale(TVector3 s)
{
	_11 = s.x; _22 = s.y; _33 = s.z;
}
void TMatrix::RotateZ(float fRadian)
{
	SetIdentity();
	float fCosTheta = cosf(fRadian);
	float fSinTheta = sinf(fRadian);
	_11 = fCosTheta;  _12 = fSinTheta;
	_21 = -fSinTheta; _22 = fCosTheta;
}
void TMatrix::RotateX(float fRadian)
{
	float fCosTheta = cosf(fRadian);
	float fSinTheta = sinf(fRadian);
	_22 = fCosTheta;  _23 = fSinTheta;
	_32 = -fSinTheta; _33 = fCosTheta;
}
void TMatrix::RotateY(float fRadian)
{
	float fCosTheta = cosf(fRadian);
	float fSinTheta = sinf(fRadian);
	_11 = fCosTheta;  _13 = -fSinTheta;
	_31 = fSinTheta; _33 = fCosTheta;
}
void TMatrix::Trans(float x, float y, float z)
{
	_41 = x; _42 = y; _43 = z;
}
void TMatrix::Trans(TVector3 t)
{
	_41 = t.x; _42 = t.y; _43 = t.z;
}
void TMatrix::SetIdentity() // 단위행렬
{
	_11 = _12 = _13 = _14 = 0.0f;
	_21 = _22 = _23 = _24 = 0.0f;
	_31 = _32 = _33 = _34 = 0.0f;
	_41 = _42 = _43 = _44 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}
TMatrix::TMatrix()
{
	SetIdentity();
}
TMatrix::TMatrix(TVector3 row1, TVector3 row2, TVector3 row3,
	TVector3 row4)
{
	_11 = row1.x; _12 = row1.y; _13 = row1.z; _14 = 0.0f;
	_21 = row2.x; _22 = row2.y; _23 = row2.z; _24 = 0.0f;
	_31 = row3.x; _32 = row3.y; _33 = row3.z; _34 = 0.0f;
	_41 = row4.x; _42 = row4.y; _43 = row4.z; _44 = 1.0f;
}
TMatrix TMatrix::CreateViewMatrix(TVector3 vPosition,
	TVector3 vTarget,
	TVector3 vUp)
{
	TMatrix m;
	TVector3 vDirection = vTarget - vPosition;
	vDirection = vDirection.Normal();
	float fDot = vUp | vDirection; // 내적
	TVector3 vC = vDirection * fDot;
	TVector3 vUpVector = vUp - (vDirection * fDot);
	vUpVector = vUpVector.Normal();
	TVector3 vRightVector = vUpVector ^ vDirection; // 외적

	m._11 = vRightVector.x;	m._12 = vUpVector.x; m._13 = vDirection.x;
	m._21 = vRightVector.y;	m._22 = vUpVector.y; m._23 = vDirection.y;
	m._31 = vRightVector.z;	m._32 = vUpVector.z; m._33 = vDirection.z;

	m._41 = -(vPosition | vRightVector);
	m._42 = -(vPosition | vUpVector);
	m._43 = -(vPosition | vDirection);
	return m;
}
// 원근투영,  직교투영	
TMatrix TMatrix::CreateProjMatrix(
	float fFov,		     // 시야각(수평)
	float fAspect,       // 화면비율(가로/세로)
	float fNearPlane,    // 근단면 거리
	float fFarPlane)	 // 원단면 거리
{
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
	return ret;
}