#include "TCollision.h"
void PRect::operator=(SRect& rt)
{
	this->x1 = rt.x;
	this->x2 = rt.x + rt.w;
	this->y1 = rt.y;
	this->y2 = rt.y + rt.h;
}
void SRect::operator=(PRect& rt)
{
	this->x = rt.x1;
	this->y = rt.y1;
	this->w = rt.x2 - rt.x1;
	this->h = rt.y2 - rt.y1;
}
bool TCollision::CheckRectToPoint(TRect& rt, POINT pt)
{
	if (rt.x <= pt.x && pt.x <= rt.x2)
	{
		if (rt.y <= pt.y && pt.y <= rt.y2)
		{
			return true;
		}
	}
	return false;
}
bool TCollision::CheckRectToPoint(SRect& rt, POINT pt)
{
	return true;
}
bool TCollision::CheckRectToPoint(PRect& rt, POINT pt)
{
	return true;
}


bool TCollision::CheckRectToRect(TRect& rt1, TRect rt2)
{
	// size
	TVector2 vMin, vMax;
	vMin.x = min(rt1.x, rt2.x);
	vMin.y = min(rt1.y, rt2.y);
	vMax.x = max(rt1.x2, rt2.x2);
	vMax.y = max(rt1.y2, rt2.y2);
	TVector2 vSize, vObjectSize;
	vSize.x = vMax.x - vMin.x;
	vSize.y = vMax.y - vMin.y;
	vObjectSize.x = rt1.w + rt2.w;
	vObjectSize.y = rt1.h + rt2.h;
	if (vSize.x <= vObjectSize.x &&
		vSize.y <= vObjectSize.y)
	{
		return true;
	}
	return false;
}
bool TCollision::CheckSphereToPoint(TSphere& s, float x, float y)
{
	return CheckSphereToPoint(s, { x,y });
}
bool TCollision::CheckSphereToPoint(TSphere& s, TVector2 p)
{
	float fDistance = (s.vCenter - p).Length();
	if (s.fRadius >= fDistance)
	{
		return true;
	}
	return false;
}
bool TCollision::CheckSphereToSphere(TSphere& s, TSphere t)
{
	float fDistance = (s.vCenter - t.vCenter).Length();
	if ((s.fRadius + t.fRadius) >= fDistance)
	{
		return true;
	}
	return false;
}