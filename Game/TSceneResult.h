#pragma once
#include "TScene.h"
class TSceneResult : public TScene
{
public:
	GUIFSM					 m_GuiFSM;
	using tUI = std::shared_ptr<ControlGUI>;
	std::vector<tUI>		m_UiList;
public:
	virtual void ProcessAction(Object* pObj);
public:
	TSceneResult(TGame* p);
	TSceneResult() { m_iState = TSceneState::SCENE_RESULT; };
	virtual ~TSceneResult();
public:
	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
};

