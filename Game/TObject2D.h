#pragma once
#include "TObject.h"
class TObject2D : public TObject
{
public:
	TVector2 ScreenToNDC(float x, float y, POINT size);
	TVector2 CameraToNDC(float x, float y, POINT size);
	virtual void	Transform(TVector2 vCamera) override;
	virtual void    TransformCamera(TVector2 vCamera);
	virtual void    TransformNDC();
	virtual void    SetVertexData();
	virtual void    SetIndexData();
public:
	TObject2D();
	TObject2D(TVector2 p);
	TObject2D(float x, float y);
};

