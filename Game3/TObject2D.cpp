#include "TObject2D.h"
TVertex2 TObject2D::ScreenToNDC(float x, float y, POINT size)
{
	TVertex2 ret = { x, y };
	ret.x = (x / size.x) * 2.0f - 1.0f; // 0 ~1
	ret.y = -((y / size.y) * 2.0f - 1.0f);
	return ret;
}