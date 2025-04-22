#pragma once
#include "Object.h"
#include "TSceneIntro.h"
#include "TSceneLobby.h"
#include "TSceneGameIn.h"
#include "TSceneResult.h"
#include "TSceneBoss.h"

class TSceneFSM : public FiniteStateMachine
{

};

class TGame : public Object
{

public:
	TScene* m_pAction = nullptr;
	static std::vector<std::shared_ptr<TScene>>  m_pActionList;
	FiniteStateMachine* m_pFsm = nullptr;
public:
	virtual void SetFSM(FiniteStateMachine* pFsm);
	static  void CreateActionFSM();
	virtual void SetTransition(UINT iEvent);
	virtual void FrameState(Object* pHero);
public:
	void		Init();
	void		Frame();
	void		Render();
	void		Release();
};

