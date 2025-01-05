#pragma once
#include "TScene.h"
class TSceneResult : public TScene
{
public:
	TGUIFSM					 m_GuiFSM;
	using tUI = std::shared_ptr<TControlGUI>;
	std::vector<tUI>		m_UiList;
public:
	virtual void ProcessAction(TObject* pObj);
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

