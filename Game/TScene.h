#pragma once
#include "Std.h"
#include "TWorld.h"
#include "Sound.h"
#include "FiniteState.h"
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
	Sound* m_pIntro = nullptr;
	Sound* m_pRobby = nullptr;
	Sound* m_pInGame = nullptr;
	Sound* m_pTeleport = nullptr;
	Sound* m_pJumpSound = nullptr;
	Sound* m_pShotSound = nullptr;
	Sound* m_pCrashSound = nullptr;
	Sound* m_pBossSound = nullptr;
	Sound* m_pClearSound = nullptr;
	Sound* m_pHurt = nullptr;
	Sound* m_pMon2M = nullptr;
	Sound* m_pMon3M = nullptr;
	Sound* m_pBossA2 = nullptr;
	Sound* m_pBossDrop = nullptr;

	vector<Sound*> m_BGMList;
	vector<Sound*> m_EffectList;
public:
	virtual void ProcessAction(Object* pObj) = 0;
public:
	TGame* m_pOwner = nullptr;
public:
	virtual void   Init() = 0;
	virtual void   Frame();
	virtual void   Render() = 0;
	virtual void   Release() = 0;
	virtual bool   CreateSound();
};

