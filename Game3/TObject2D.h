#pragma once
#include "TObject.h"
class TObject2D : public TObject
{
public:
	TVertex2 ScreenToNDC(float x, float y, POINT size);
};

