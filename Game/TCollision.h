#pragma once
#include "TVertex.h"

struct PRect;
struct SRect
{
	float   x, y;
	float   w, h;
	void operator = (PRect& pt);
};
struct PRect
{
	float   x1, y1;
	float   x2, y2;
	void operator = (SRect& pt);
};
struct TRect
{
	float   x, y;
	float   x2, y2;
	float   w, h;
	float   fRadius;
	TVector2  tCenter;
	void    SetP(float x1, float y1, float x2, float y2)
	{
		SetP({ x1,y1 }, { x2,y2 });
	}
	void    SetS(float x1, float y1, float w, float h)
	{
		SetS({ x1,y1 }, { w,h });
	}
	void    SetP(TVector2 v1, TVector2 v2)
	{
		x = v1.x; y = v1.y;
		this->x2 = v2.x; this->y2 = v2.y;
		w = x2 - x;
		h = y2 - y;
		tCenter.x = (x2 + x) / 2.0f;
		tCenter.y = (y2 + y) / 2.0f;
		TVector2 vMin = { x,y };
		TVector2 vMax = { x2, y2 };
		fRadius = (vMax - vMin).Length() * 0.5f;
	}
	void    SetS(TVector2 v1, TVector2 v2)
	{
		x = v1.x; y = v1.y;
		this->w = v2.x; this->h = v2.y;
		x2 = x + w;
		y2 = y + h;
		tCenter.x = (x2 + x) / 2.0f;
		tCenter.y = (y2 + y) / 2.0f;
		TVector2 vMin = { x,y };
		TVector2 vMax = { x2, y2 };
		fRadius = (vMax - vMin).Length() * 0.5f;
	}
	TRect()
	{
		x = y = x2 = y2 = w = h = 0.0f;
	}
};

struct TSphere
{
	TVector2 vCenter;
	float    fRadius;
};


class TCollision
{
public:
	static bool CheckRectToPoint(TRect& rt, POINT pt);
	static bool CheckRectToPoint(PRect& rt, POINT pt);
	static bool CheckRectToPoint(SRect& rt, POINT pt);
	static bool CheckRectToRect(TRect& rt1, TRect rt2);
	static bool CheckSphereToPoint(TSphere& s, float x, float y);
	static bool CheckSphereToPoint(TSphere& s, TVector2 p);
	static bool CheckSphereToSphere(TSphere& rt1, TSphere rt2);
};

