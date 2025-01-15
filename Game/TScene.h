#pragma once
#include "TStd.h"
#include "TWorld.h"
#include "TSound.h"
#include "TFiniteState.h"
enum TSceneState
{
	SCENE_INTRO = 0,
	SCENE_LOBBY,
	SCENE_INGAME,
	SCENE_BOSS,
	SCENE_RESULT,
	SCENE_FINAL,
	SCENE_COUNT,
};
// 상태전이
enum TSceneEvent
{
	EVENT_NEXT_SCENE = 0,
	EVENT_PREV_SCENE,
	EVENT_SCENE_COUNT,
	EVENT_BOSS_DIED,
};




class TGame;
class TScene
{
	static std::vector<std::shared_ptr<TScene>>  m_pActionList;
public:
	bool		  m_bNextScene = false;
	bool		  m_bPrevScene = false;
	bool          m_bLoadPrevScene = false;
	bool          m_bLoadNextScene = false;
	std::shared_ptr<TWorld>  m_pWorld;
public:
	UINT   m_iState = 0;
	TScene(TGame* p);
	TScene();
	virtual ~TScene();
public:
	TSound* m_pIntro = nullptr;
	TSound* m_pRobby = nullptr;
	TSound* m_pInGame = nullptr;
	TSound* m_pTeleport = nullptr;
	TSound* m_pJumpSound = nullptr;
	TSound* m_pShotSound = nullptr;
	TSound* m_pCrashSound = nullptr;
	TSound* m_pBossSound = nullptr;
	TSound* m_pClearSound = nullptr;

	vector<TSound*> m_BGMList;
	vector<TSound*> m_EffectList;
public:
	virtual void ProcessAction(TObject* pObj) = 0;
public:
	TGame* m_pOwner = nullptr;
public:
	virtual void   Init() = 0;
	virtual void   Frame() = 0;
	virtual void   Render() = 0;
	virtual void   Release() = 0;
	virtual bool   CreateSound();
};

