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
	_31 = _32  = 0.0f;
	_11 = _22 = _33 = 1.0f;
}
TMatrix3::TMatrix3()
{
	SetIdentity();
}
TMatrix3::TMatrix3(TVector2 row1, TVector2 row2, TVector2 row3)
{
	_11 = row1.x; _12 = row1.y; _13 = 0.0f;
	_21 = row2.x; _22 = row2.y; _23 = 0.0f;
	_31 = row3.x; _32 = row3.y; _33 = 1.0f;	
}