#pragma once
#include "TWindow.h"
class TGameCore : public TWindow
{
public:
public:
	virtual void   Init() {};
	virtual void   Frame() {};
	virtual void   Render() {};
	virtual void   Release() {};
public:
	void   CoreInit();
	void   CoreFrame();
	void   CoreRender();
	void   CoreRelease();
	virtual bool   GameRun();
};

