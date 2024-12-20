#pragma once
#include "TVertex.h"

struct PRect;
struct tPoint
{
	float x;
	float y;
};
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
	tPoint  tCenter;
	void    SetP(float x1, float y1, float x2, float y2)
	{
		x = x1; y = y1;
		this->x2 = x2; this->y2 = y2;
		w = x2 - x1;
		h = y2 - y1;
		tCenter.x = (x2 + x) / 2.0f;
		tCenter.y = (y2 + y) / 2.0f;
	}
	void    SetS(float x1, float y1, float w, float h)
	{
		x = x1; y = y1;
		this->w = w; this->h = h;
		x2 = x1 + w;
		y2 = y1 + h;
		tCenter.x = (x2 + x) / 2.0f;
		tCenter.y = (y2 + y) / 2.0f;
	}
	void    SetP(TVector2 v1, TVector2 v2)
	{
		x = v1.x; y = v1.y;
		this->x2 = v2.x; this->y2 = v2.y;
		w = x2 - x;
		h = y2 - y;
		tCenter.x = (x2 + x) / 2.0f;
		tCenter.y = (y2 + y) / 2.0f;
	}
	void    SetS(TVector2 v1, TVector2 v2)
	{
		x = v1.x; y = v1.y;
		this->w = v2.x; this->h = v2.y;
		x2 = x + w;
		y2 = y + h;
		tCenter.x = (x2 + x) / 2.0f;
		tCenter.y = (y2 + y) / 2.0f;
	}
	TRect()
	{
		x = y = x2 = y2 = w = h = 0.0f;
	}
};


class TCollision
{
public:
	static bool CheckRectToPoint(TRect& rt, POINT pt);
	static bool CheckRectToPoint(PRect& rt, POINT pt);
	static bool CheckRectToPoint(SRect& rt, POINT pt);
};

