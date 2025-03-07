#pragma once
#include "TVector.h"
struct float3x3
{
	union {
		struct {
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
			//float _41, _42, _43;
		};
		float m[3][3];
	};
};
struct float4x4
{
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
class TMatrix3 : public float3x3
{
public:
	void SetIdentity(); // 단위행렬
	void Scale(float x, float y);
	void Scale(TVector2 s);
	void Rotate(float fRadian);
	void Trans(float x, float y);
	void Trans(TVector2 s);
	TMatrix3 Transpose();// 전치행렬
public:
	TMatrix3 operator *(const TMatrix3& m);
public:
	TMatrix3();
	TMatrix3(TVector2 row1, TVector2 row2, TVector2 row3);
};

class TMatrix : public float4x4
{
public:
	void SetIdentity(); // 단위행렬
	void Scale(float x, float y, float z);
	void Scale(TVector3 s);
	void RotateX(float fRadian);
	void RotateY(float fRadian);
	void RotateZ(float fRadian);
	void Trans(float x, float y, float z);
	void Trans(TVector3 s);
	TMatrix Transpose();// 전치행렬
public:
	TMatrix operator *(const TMatrix& m);
public:
	static TMatrix CreateViewMatrix(
		TVector3 vCameraPos, // 카메라 위치
		TVector3 vCameraTarget, // 카메라가 바라보는 위치
		TVector3 vCameraUp); // 카메라의 위쪽 방향
	static TMatrix CreateProjMatrix(
		float fFov,		// 시야각(수평)
		float fAspect,  // 화면비율(가로/세로)
		float fNear,    // 근단면 거리
		float fFar);		// 원단면 거리;
public:
	TMatrix();
	TMatrix(TVector3 row1, TVector3 row2, TVector3 row3, TVector3 row4);
};



