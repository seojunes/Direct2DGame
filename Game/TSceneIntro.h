#pragma once
#include "TScene.h"
class TSceneIntro : public TScene
{
public:
	TGUIFSM					 m_GuiFSM;
	using tUI = std::shared_ptr<TControlGUI>;
	std::vector<tUI>		m_UiList;
public:
	virtual void ProcessAction(TObject* pObj);
public:
	float m_fEscapeTime = 3.0f;
public:
	TSceneIntro(TGame* p);
	TSceneIntro() { m_iState = TSceneState::SCENE_INTRO; };
	virtual ~TSceneIntro();
public:
	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
};

