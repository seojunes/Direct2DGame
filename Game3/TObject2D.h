#pragma once
#include "TObject.h"
class TObject2D : public TObject
{
public:
	TVertex2 ScreenToNDC(float x, float y, POINT size);
	TVertex2 CameraToNDC(float x, float y, POINT size);

	virtual void	Transform(TVertex2 vCamera) override;
	virtual void    TransformCamera(TVertex2 vCamera);
	virtual void    TransformNDC();
	virtual void    SetVertexData();
	virtual void    SetIndexData();
};

