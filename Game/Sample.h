#pragma once
#include "GameCore.h"
#include "TGame.h"

class Sample : public GameCore
{
	TGame			m_Game;
	TSceneFSM	    m_SceneFSM;
public:
	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
};

