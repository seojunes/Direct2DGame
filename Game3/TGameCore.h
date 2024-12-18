#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTime.h"
#include "TInput.h"
#include "TDxWrite.h"
#include "TObject.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
class TGameCore : public TWindow
{	
public:
	TTime		m_GameTimer;
	TInput		m_Input;
	TDevice		m_DxDevice;
	TDxWrite    m_DxWrite;
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

