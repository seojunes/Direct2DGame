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