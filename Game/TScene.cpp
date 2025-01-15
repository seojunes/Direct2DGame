#include "TScene.h"
TScene::TScene(TGame* p) : m_pOwner(p)
{

}
TScene::TScene()
{
};
TScene::~TScene()
{

}

bool TScene::CreateSound()
{
	TSoundManager& mgr = TSoundManager::GetInstance();
	m_pIntro = mgr.Load(L"../../data/sound/game over jingle.ogg");
	m_BGMList.emplace_back(m_pIntro);
	m_pRobby = mgr.Load(L"../../data/sound/Robby.mp3");
	m_BGMList.emplace_back(m_pIntro);
	m_pInGame = mgr.Load(L"../../data/sound/GameIn.mp3");
	m_BGMList.emplace_back(m_pIntro);
	m_pTeleport = mgr.Load(L"../../data/sound/TeleportAble.wav");
	m_EffectList.emplace_back(m_pIntro);
	m_pJumpSound = mgr.Load(L"../../data/sound/Jump.ogg");
	m_EffectList.emplace_back(m_pIntro);
	m_pShotSound = mgr.Load(L"../../data/sound/Laser Fire.wav");
	m_EffectList.emplace_back(m_pIntro);
	m_pCrashSound = mgr.Load(L"../../data/sound/Crash.wav");
	m_EffectList.emplace_back(m_pIntro);
	m_pBossSound = mgr.Load(L"../../data/sound/Boss.mp3");
	m_BGMList.emplace_back(m_pIntro);
	m_pClearSound = mgr.Load(L"../../data/sound/StageClear.mp3");
	m_BGMList.emplace_back(m_pIntro);
	return true;
}