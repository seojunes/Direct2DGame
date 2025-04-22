#pragma once
#include "Window.h"
#include "Device.h"
#include "Time.h"
#include "Input.h"
#include "DxWrite.h"
#include "Object.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
class GameCore : public Window
{
public:
	Time		m_GameTimer;
	Input		m_Input;
	Device		m_DxDevice;
	DxWrite    m_DxWrite;
	static MeshRender m_MeshRender;


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

