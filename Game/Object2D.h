#pragma once
#include "Object.h"
class Object2D : public Object
{
public:
	float m_fZoom = 1.0f;
	TVector2 ScreenToNDC(float x, float y, POINT size);
	TVector2 CameraToNDC(float x, float y, POINT size);
	virtual void	Transform(TVector2 vCamera) override;
	virtual void    TransformCamera(TVector2 vCamera);
	virtual void    TransformNDC();
public:
	Object2D();
	Object2D(TVector2 p);
	Object2D(float x, float y);
public:
	void SetZoom(float zoom);
};

