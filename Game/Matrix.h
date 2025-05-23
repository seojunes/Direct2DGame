#pragma once
#include "TVector.h"
struct float3x3
{
	union {
		struct {
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float m[3][3];
	};
};
class Matrix3 : public float3x3
{
public:
	void SetIdentity(); // 단위행렬
	void Scale(float x, float y);
	void Scale(TVector2 s);
	void Rotate(float fRadian);
	void Trans(float x, float y);
	void Trans(TVector2 s);
	Matrix3 Transpose();// 전치행렬
public:
	Matrix3 operator *(const Matrix3& m);
public:
	Matrix3();
	Matrix3(TVector2 row1, TVector2 row2, TVector2 row3);
};

