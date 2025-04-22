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
	SoundManager& mgr = SoundManager::GetInstance();
	m_pIntro = mgr.Load(L"../../data/sound/game over jingle.ogg");
	m_BGMList.emplace_back(m_pIntro);
	m_pRobby = mgr.Load(L"../../data/sound/Robby.mp3");
	m_BGMList.emplace_back(m_pRobby);
	m_pInGame = mgr.Load(L"../../data/sound/GameIn.mp3");
	m_BGMList.emplace_back(m_pInGame);
	m_pTeleport = mgr.Load(L"../../data/sound/TeleportAble.wav");
	m_EffectList.emplace_back(m_pTeleport);
	m_pJumpSound = mgr.Load(L"../../data/sound/Jump.ogg");
	m_EffectList.emplace_back(m_pJumpSound);
	m_pShotSound = mgr.Load(L"../../data/sound/Laser Fire.wav");
	m_EffectList.emplace_back(m_pShotSound);
	m_pCrashSound = mgr.Load(L"../../data/sound/Crash.wav");
	m_EffectList.emplace_back(m_pCrashSound);
	m_pBossSound = mgr.Load(L"../../data/sound/Boss.mp3");
	m_BGMList.emplace_back(m_pBossSound);
	m_pClearSound = mgr.Load(L"../../data/sound/StageCleard.mp3");
	m_BGMList.emplace_back(m_pClearSound);
	m_pHurt = mgr.Load(L"../../data/sound/hurt.wav");
	m_EffectList.emplace_back(m_pHurt);
	m_pMon2M = mgr.Load(L"../../data/sound/Mon2M.wav");
	m_EffectList.emplace_back(m_pMon2M);
	m_pMon3M = mgr.Load(L"../../data/sound/bomb.wav");
	m_EffectList.emplace_back(m_pMon3M);
	m_pBossA2 = mgr.Load(L"../../data/sound/BossA2.wav");
	m_EffectList.emplace_back(m_pBossA2);
	m_pBossDrop = mgr.Load(L"../../data/sound/BossDrop.wav");
	m_EffectList.emplace_back(m_pBossDrop);
	m_pClearSound->m_fVolume = 0.02f;
	m_pIntro->m_fVolume = 0.05f;
	m_pRobby->m_fVolume = 0.05f;
	m_pInGame->m_fVolume = 0.05f;
	m_pTeleport->m_fVolume = 0.05f;
	m_pBossSound->m_fVolume = 0.05f;
	m_pCrashSound->m_fVolume = 0.15f;

	
	return true;
}

void TScene::Frame()
{
	if (g_GameKey.dw1key == KEY_HOLD)
	{
		for (auto data : m_BGMList)
		{
			data->VolumeUp(5*g_fSPF);			
		}
	}
	else if (g_GameKey.dw2key == KEY_HOLD)
	{
		for (auto data : m_BGMList)
		{
			data->VolumeDown(5 * g_fSPF);
		}
	}
	else if (g_GameKey.dw3key == KEY_HOLD)
	{
		for (auto data : m_EffectList)
		{
			data->VolumeUp(5 * g_fSPF);
		}
	}
	else if (g_GameKey.dw4key == KEY_HOLD)
	{
		for (auto data : m_EffectList)
		{
			data->VolumeDown(5 * g_fSPF);
		}
	}
}