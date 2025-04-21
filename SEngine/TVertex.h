#pragma once
#include "TVector.h"
struct P_VERTEX
{
	TVector2 v;
};
struct PC_VERTEX
{
	TVector2 v;
	TVector4 c;
};
struct PCT_VERTEX
{
	TVector2 v;
	TVector4 c;
	TVector2 t;
};
struct PNCT_VERTEX
{
	TVector3 p;
	TVector3 n;
	TVector4 c;
	TVector2 t;
	PNCT_VERTEX() {}
	PNCT_VERTEX(TVector3 p, TVector3 n, TVector4 c,
		TVector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};
struct IW_VERTEX
{
	float	i1[4];// I0, I1, I2, I3
	float	w1[4];// W0, fW1, fW2, fNumWeight;	
	float	i2[4];// I0, I1, I2, I3
	float	w2[4];// W0, fW1, fW2, fNumWeight;	
	IW_VERTEX()
	{
		i1[3] = i1[2] = i1[1] = i1[0] = 0.0f;
		w1[3] = w1[2] = w1[1] = w1[0] = 0.0f;
		i2[3] = i2[2] = i2[1] = i2[0] = 0.0f;
		w2[3] = w2[2] = w2[1] = w2[0] = 0.0f;
	}
};
