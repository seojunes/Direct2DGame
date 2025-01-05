#include "TCollision.h"
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
bool TCollision::CheckRectToPoint(TRect& rt, TVector2 pt)
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
bool TCollision::CheckRectToPoint(TRect& rt, POINT pt)
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

//CollisionDirection TCollision::DetectCollisionDirection(const TRect& heroRect, const TRect& rectBox) {
//	float overlapLeft = heroRect.v2.x - rectBox.v1.x;  // ���� ��ħ
//	float overlapRight = rectBox.v2.x - heroRect.v1.x; // ������ ��ħ
//	float overlapTop = heroRect.v2.y - rectBox.v1.y;   // ���� ��ħ
//	float overlapBottom = rectBox.v2.y - heroRect.v1.y; // �Ʒ��� ��ħ
//
//	// �ּ� ��ħ ���� �Ǻ�
//	float minOverlap = min( overlapLeft, overlapRight, overlapTop, overlapBottom );
//
//	if (minOverlap == overlapBottom) 
//	{
//		return CollisionDirection::Bottom; // �ٴ� �浹
//	}
//	else if (minOverlap == overlapTop) 
//	{
//		return CollisionDirection::Top; // �� �浹
//	}
//	else if (minOverlap == overlapLeft) 
//	{
//		return CollisionDirection::Left; // ���� �浹
//	}
//	else if (minOverlap == overlapRight) 
//	{
//		return CollisionDirection::Right; // ������ �浹
//	}
//
//	return CollisionDirection::None;
//}