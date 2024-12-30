#pragma once
#include "TMatrix.h"

class TObject;
enum TCollisionType
{
	T_Block = 0,
	T_Overlap, // ��ħ
	T_Ignore,  // ����
};

struct THitResult
{
	TObject* pObject;
	TVector2 vPos;
	TVector2 vDir;
};

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
	TVector2 v1;// �»��
	TVector2 v2;// ���ϱ�
	TVector2 vs;// ����,����ũ��
	TVector2 vh;// ����,���� ����ũ��
	TVector2 vc;// �߽�
	float    fR;// �Ÿ�(�밢��)
	void    Size(float w, float h)
	{
		SetS(v1, { w,h });
	}
	void    Size(TVector2 s)
	{
		SetS(v1, s);
	}
	void    Move(float x, float y)
	{
		vc.x = x;
		vc.y = y;
		v1 = vc - vh;
		v2 = vc + vh;
	}
	void    Move(TVector2 p)
	{
		Move(p.x, p.y);
	}
	void    SetP(float x1, float y1, float x2, float y2)
	{
		SetP({ x1,y1 }, { x2,y2 });
	}
	void    SetS(float x1, float y1, float w, float h)
	{
		SetS({ x1,y1 }, { w,h });
	}
	void    SetP(TVector2 s, TVector2 e)
	{
		v1 = s;
		v2 = e;
		vs = v2 - v1;
		vh = vs / 2.0f;
		fR = (v2 - v1).Length() * 0.5f;
		Move((v2 + v1) / 2.0f);
	}
	void    SetS(TVector2 p, TVector2 s)
	{
		v1 = p;
		vs = s;
		v2 = v1 + vs;
		vh = vs / 2.0f;
		fR = (v2 - v1).Length() * 0.5f;
		Move((v2 + v1) / 2.0f);
	}
	TRect()
	{
		fR = 0.0f;
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
	static bool CheckRectToPoint(TRect& rt, TVector2 pt);
};

