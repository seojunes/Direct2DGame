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

