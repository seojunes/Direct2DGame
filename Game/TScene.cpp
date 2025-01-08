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
	m_pSound = mgr.Load(L"../../data/sound/game over jingle.ogg");
	return true;
}