#include "TVector.h"
#include "Matrix.h"
TVector2 TVector2::operator * (const Matrix3& m)
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
	if (Length() > 0.0f) 
	{
		float fInvertLength = 1.0f / Length();
		x *= fInvertLength;
		y *= fInvertLength;
	}
	else
	{
		x = 0.0f;
		y = 0.0f; 
	}
}
TVector2   TVector2::Normal()
{
	if (Length() != 0)
	{
		float fInvertLength = 1.0f / Length();
		return TVector2(x * fInvertLength, y * fInvertLength);
	}	
	else
	{
		return TVector2(0, 0);
	}	
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