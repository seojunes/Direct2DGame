#pragma once
#include "TVector.h"
struct float3x3
{
	union {
		struct {
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
			float _41, _42, _43;
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
	void Transpose();// 전치행렬
public:
	TMatrix operator *(const TMatrix& m);
public:
	static TMatrix Transpose(const TMatrix& m);

public:
	TMatrix();
	TMatrix(TVector3 row1, TVector3 row2, TVector3 row3, TVector3 row4);
};
// 사원수
class TQuaternion
{
public:
	float x, y, z, w;
};

