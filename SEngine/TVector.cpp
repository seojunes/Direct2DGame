#include "TVector.h"
#include "TMatrix.h"
TVector2 TVector2::operator * (const TMatrix3& m)
{
	TVector2 ret = *this;
	ret.x = x * m._11 + y * m._21 + 1.0f * m._31;
	ret.y = x * m._12 + y * m._22 + 1.0f * m._32;
	return ret;
}
void TVector2::operator +=(const TVector2& v)
{
	x = x + v.x;
	y = y + v.y;
}
void TVector2::operator -=(const TVector2& v)
{
	x = x - v.x;
	y = y - v.y;
}

TVector2 TVector2::operator +(const TVector2& v)
{
	TVector2 ret;
	ret.x = x + v.x;
	ret.y = y + v.y;
	return ret;
}
TVector2 TVector2::operator -(const TVector2& v)
{
	TVector2 ret;
	ret.x = x - v.x;
	ret.y = y - v.y;
	return ret;
}
TVector2 TVector2::operator /(const float s)
{
	TVector2 ret;
	ret.x = x / s;
	ret.y = y / s;
	return ret;
}
TVector2 TVector2::operator *(const float s)
{
	TVector2 ret;
	ret.x = x * s;
	ret.y = y * s;
	return ret;
}
bool TVector2::operator ==(const TVector2& v)
{
	if (fabs(x - v.x) < T_Epsilon)
	{
		if (fabs(y - v.y) < T_Epsilon)
		{
			return true;
		}
	}
	return false;
}
bool TVector2::operator !=(const TVector2& v)
{
	if (*this == v)
		return false;
	return true;
}
float  TVector2::Length()
{
	float fDistance = sqrt(x * x + y * y);
	return fDistance;
}
void   TVector2::Normalize()
{
	float fInvertLength = 1.0f / Length();
	x *= fInvertLength;
	y *= fInvertLength;
	//Length() == 1.0f
}
TVector2   TVector2::Normal()
{
	float fInvertLength = 1.0f / Length();
	return TVector2(x * fInvertLength, y * fInvertLength);
}

TVector2::TVector2()
{
	x = y = 0.0f;
}
TVector2::TVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

/// <summary>
/// 
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
TVector3 TVector3::operator * (const TMatrix& m)
{
	TVector3 ret = *this;
	ret.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
	ret.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
	ret.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
	return ret;
}
void TVector3::operator +=(const TVector3& v)
{
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
}
void TVector3::operator -=(const TVector3& v)
{
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
}

TVector3 TVector3::operator +(const TVector3& v)
{
	TVector3 ret;
	ret.x = x + v.x;
	ret.y = y + v.y;
	ret.z = z + v.z;
	return ret;
}
TVector3 TVector3::operator -(const TVector3& v)
{
	TVector3 ret;
	ret.x = x - v.x;
	ret.y = y - v.y;
	ret.z = z - v.z;
	return ret;
}
TVector3 TVector3::operator /(const float s)
{
	TVector3 ret;
	ret.x = x / s;
	ret.y = y / s;
	ret.z = z / s;
	return ret;
}
TVector3 TVector3::operator *(const float s)
{
	TVector3 ret;
	ret.x = x * s;
	ret.y = y * s;
	ret.z = z * s;
	return ret;
}
bool TVector3::operator ==(const TVector3& v)
{
	if (fabs(x - v.x) < T_Epsilon)
	{
		if (fabs(y - v.y) < T_Epsilon)
		{
			if (fabs(z - v.z) < T_Epsilon)
			{
				return true;
			}
		}
	}
	return false;
}
bool TVector3::operator !=(const TVector3& v)
{
	if (*this == v)
		return false;
	return true;
}
float  TVector3::Length()
{
	float fDistance = sqrt(x * x + y * y + z * z);
	return fDistance;
}
void   TVector3::Normalize()
{
	float fInvertLength = 1.0f / Length();
	x *= fInvertLength;
	y *= fInvertLength;
	z *= fInvertLength;
}
TVector3   TVector3::Normal()
{
	float fInvertLength = 1.0f / Length();
	return TVector3(x * fInvertLength, y * fInvertLength, z * fInvertLength);
}

TVector3::TVector3()
{
	x = y = z = 0.0f;
}
TVector3::TVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
float TVector3::operator | (const TVector3& v)
{
	return x * v.x + y * v.y + z * v.z;
}
TVector3 TVector3::operator ^ (const TVector3& v)
{
	TVector3 ret;
	ret.x = y * v.z - z * v.y;
	ret.y = z * v.x - x * v.z;
	ret.z = x * v.y - y * v.x;
	return ret;
}

////
TVector4 TVector4::operator +(const TVector4& v)
{
	TVector4 ret;
	ret.x = x + v.x;
	ret.y = y + v.y;
	ret.z = z + v.z;
	ret.w = w + v.w;
	return ret;
}
TVector4 TVector4::operator -(const TVector4& v)
{
	TVector4 ret;
	ret.x = x - v.x;
	ret.y = y - v.y;
	ret.z = z - v.z;
	ret.w = w - v.w;
	return ret;
}
TVector4 TVector4::operator /(const float s)
{
	TVector4 ret;
	ret.x = x / s;
	ret.y = y / s;
	ret.z = z / s;
	ret.w = w / s;
	return ret;
}
TVector4 TVector4::operator *(const float s)
{
	TVector4 ret;
	ret.x = x * s;
	ret.y = y * s;
	ret.z = z * s;
	ret.w = w * s;
	return ret;
}
bool TVector4::operator ==(const TVector4& v)
{
	if (fabs(x - v.x) < T_Epsilon)
	{
		if (fabs(y - v.y) < T_Epsilon)
		{
			if (fabs(z - v.z) < T_Epsilon)
			{
				if (fabs(w - v.w) < T_Epsilon)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool TVector4::operator !=(const TVector4& v)
{
	if (*this == v)
		return false;
	return true;
}
float  TVector4::Length()
{
	float fDistance = sqrt(x * x + y * y + z * z + w * w);
	return fDistance;
}
void   TVector4::Normalize()
{
	float fInvertLength = 1.0f / Length();
	x *= fInvertLength;
	y *= fInvertLength;
	z *= fInvertLength;
	w *= fInvertLength;
	//Length() == 1.0f
}
TVector4   TVector4::Normal()
{
	float fInvertLength = 1.0f / Length();
	return TVector4(x * fInvertLength, y * fInvertLength,
		z * fInvertLength, w * fInvertLength);
}

TVector4::TVector4()
{
	x = y = z = w = 0.0f;
}
TVector4::TVector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}