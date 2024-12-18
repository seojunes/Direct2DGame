#pragma once
#include <Windows.h>
struct TVertex2
{
	float x, y;
};
struct TVertex3
{
	float x, y, z;
};
struct TVertex4
{
	float x, y, z, w;
};
struct P_VERTEX
{
	TVertex2 v;
};
struct PC_VERTEX
{
	TVertex2 v;
	TVertex4 c;
};
struct PCT_VERTEX
{
	TVertex2 v;
	TVertex4 c;
	TVertex2 t;
};

