#pragma once
#include "TScene.h"
class TSceneLobby : public TScene
{
public:
	GUIFSM					 m_GuiFSM;
	using tUI = std::shared_ptr<ControlGUI>;
	std::vector<tUI>		m_UiList;
public:
	TSceneLobby(TGame* p);
	TSceneLobby() { m_iState = TSceneState::SCENE_LOBBY; };
	virtual ~TSceneLobby();
	void ProcessAction(Object* pObj);
public:
	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
};

