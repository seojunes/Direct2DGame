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

class Matrix : public float4x4
{
	static Matrix m_matIdentity;
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
	Matrix operator *(const Matrix& m);
public:
	static Matrix Transpose(const Matrix& m);
	static Matrix Identity();

public:
	Matrix();
	Matrix(TVector3 row1, TVector3 row2, TVector3 row3, TVector3 row4);
};
// 쿼터니안
class TQuaternion
{
public:
	float x, y, z, w;
};

