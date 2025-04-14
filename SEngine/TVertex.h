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
	float	i[4];// I0, I1, I2, I3
	float	w[4];// W0, fW1, fW2, fNumWeight;

	IW_VERTEX()
	{
		i[3] = i[2] = i[1] = i[0] = 0.0f;
		w[3] = w[2] = w[1] = w[0] = 0.0f;
	}
};
