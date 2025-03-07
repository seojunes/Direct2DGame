#pragma once
#include <windows.h>
#include <math.h>
#define T_Epsilon ((float)0.0001f)
#define T_Pi ((float)3.141592f)
class TMatrix3;
class TMatrix;
class TVector2
{
public:
	float  x;
	float  y;
public:
	void operator +=(const TVector2& v);
	void  operator -=(const TVector2& v);
	TVector2 operator +(const TVector2& v);
	TVector2 operator -(const TVector2& v);
	TVector2 operator /(const float s);
	TVector2 operator *(const float s);
	bool operator ==(const TVector2& v);
	bool operator !=(const TVector2& v);
	TVector2 operator * (const TMatrix3& m);
public:
	float  Length();
	void   Normalize();
	TVector2   Normal();
public:
	TVector2();
	TVector2(float x, float y);
};
class TVector3
{
public:
	float  x;
	float  y;
	float  z;
public:
	void operator +=(const TVector3& v);
	void  operator -=(const TVector3& v);
	TVector3 operator +(const TVector3& v);
	TVector3 operator -(const TVector3& v);
	TVector3 operator /(const float s);
	TVector3 operator *(const float s);
	bool operator ==(const TVector3& v);
	bool operator !=(const TVector3& v);
	TVector3 operator * (const TMatrix& m);
	float operator | (const TVector3& v); // 내적
	TVector3 operator ^ (const TVector3& v); // 외적
public:
	float  Length();
	void   Normalize();
	TVector3   Normal();
	TVector3(float x, float y, float z);
	TVector3();
};
class TVector4
{
public:
	float  x;
	float  y;
	float  z;
	float  w;
public:
	TVector4 operator +(const TVector4& v);
	TVector4 operator -(const TVector4& v);
	TVector4 operator /(const float s);
	TVector4 operator *(const float s);
	bool operator ==(const TVector4& v);
	bool operator !=(const TVector4& v);
public:
	float  Length();
	void   Normalize();
	TVector4   Normal();
public:
	TVector4();
	TVector4(float x, float y, float z, float w);
};

