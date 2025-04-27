#include "Collision.h"
void PRect::operator=(SRect& rt)
{
	this->x1 = rt.x;
	this->x2 = rt.y;
	this->y1 = rt.x + rt.w;
	this->y2 = rt.y + rt.h;
}
void SRect::operator=(PRect& rt)
{
	this->x = rt.x1;
	this->y = rt.y1;
	this->w = rt.x2 - rt.x1;
	this->h = rt.y2 - rt.y1;
}
bool Collision::CheckRectToPoint(Rect& rt, TVector2 pt)
{
	if (rt.v1.x <= pt.x && pt.x <= rt.v2.x)
	{
		if (rt.v1.y <= pt.y && pt.y <= rt.v2.y)
		{
			return true;
		}
	}
	return false;
}
bool Collision::CheckRectToPoint(Rect& rt, POINT pt)
{
	if (rt.v1.x <= (float)pt.x && (float)pt.x <= rt.v2.x)
	{
		if (rt.v1.y <= (float)pt.y && (float)pt.y <= rt.v2.y)
		{
			return true;
		}
	}
	return false;
}
bool Collision::CheckRectToPoint(SRect& rt, POINT pt)
{
	return true;
}
bool Collision::CheckRectToPoint(PRect& rt, POINT pt)
{
	return true;
}

bool Collision::CheckRectToRect(Rect& rt1, Rect rt2)
{
	// size
	TVector2 vMin, vMax;
	vMin.x = min(rt1.v1.x, rt2.v1.x);
	vMin.y = min(rt1.v1.y, rt2.v1.y);
	vMax.x = max(rt1.v2.x, rt2.v2.x);
	vMax.y = max(rt1.v2.y, rt2.v2.y);
	TVector2 vSize, vObjectSize;
	vSize.x = vMax.x - vMin.x;
	vSize.y = vMax.y - vMin.y;
	vObjectSize.x = rt1.vs.x + rt2.vs.x;
	vObjectSize.y = rt1.vs.y + rt2.vs.y;
	if (vSize.x <= vObjectSize.x &&
		vSize.y <= vObjectSize.y)
	{
		return true;
	}
	return false;
}
bool Collision::CheckSphereToPoint(Sphere& s, float x, float y)
{
	return CheckSphereToPoint(s, { x,y });
}
bool Collision::CheckSphereToPoint(Sphere& s, TVector2 p)
{
	float fDistance = (s.vCenter - p).Length();
	if (s.fRadius >= fDistance)
	{
		return true;
	}
	return false;
}
bool Collision::CheckSphereToSphere(Sphere& s, Sphere t)
{
	float fDistance = (s.vCenter - t.vCenter).Length();
	if ((s.fRadius + t.fRadius) >= fDistance)
	{
		return true;
	}
	return false;
}

//CollisionDirection Collision::DetectCollisionDirection(const Rect& heroRect, const Rect& rectBox) {
//	float overlapLeft = heroRect.v2.x - rectBox.v1.x;  // 왼쪽 겹침
//	float overlapRight = rectBox.v2.x - heroRect.v1.x; // 오른쪽 겹침
//	float overlapTop = heroRect.v2.y - rectBox.v1.y;   // 위쪽 겹침
//	float overlapBottom = rectBox.v2.y - heroRect.v1.y; // 아래쪽 겹침
//
//	// 최소 겹침 방향 판별
//	float minOverlap = min( overlapLeft, overlapRight, overlapTop, overlapBottom );
//
//	if (minOverlap == overlapBottom) 
//	{
//		return CollisionDirection::Bottom; // 바닥 충돌
//	}
//	else if (minOverlap == overlapTop) 
//	{
//		return CollisionDirection::Top; // 위 충돌
//	}
//	else if (minOverlap == overlapLeft) 
//	{
//		return CollisionDirection::Left; // 왼쪽 충돌
//	}
//	else if (minOverlap == overlapRight) 
//	{
//		return CollisionDirection::Right; // 오른쪽 충돌
//	}
//
//	return CollisionDirection::None;
//}