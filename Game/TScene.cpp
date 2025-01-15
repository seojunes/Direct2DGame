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
	m_pRobby = mgr.Load(L"../../data/sound/Robby.mp3");
	return true;
}