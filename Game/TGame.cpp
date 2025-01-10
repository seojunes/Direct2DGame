#include "TGame.h"
std::vector<std::shared_ptr<TScene>>  TGame::m_pActionList;
void TGame::SetFSM(TFiniteStateMachine* pFsm)
{
	m_pFsm = pFsm;
	m_pAction = m_pActionList[SCENE_INTRO].get();
}
void TGame::CreateActionFSM()
{
	if (m_pActionList.size()) return;
	std::shared_ptr<TScene> intro = std::make_shared<TSceneIntro>();
	std::shared_ptr<TScene> lobby = std::make_shared<TSceneLobby>();
	std::shared_ptr<TScene> InGame = std::make_shared<TSceneGameIn>();
	std::shared_ptr<TScene> Boss = std::make_shared<TSceneBoss>();
	std::shared_ptr<TScene> Result = std::make_shared<TSceneResult>();

	intro->Init();
	lobby->Init();
	//InGame->Init();
	Boss->Init();
	Result->Init();

	m_pActionList.emplace_back(intro);
	m_pActionList.emplace_back(lobby);
	m_pActionList.emplace_back(InGame);
	m_pActionList.emplace_back(Boss);
	m_pActionList.emplace_back(Result);
}
void TGame::SetTransition(UINT iEvent)
{
	_ASSERT(m_pFsm);
	UINT iOutput = m_pFsm->GetOutputState(m_pAction->m_iState, iEvent);

	// ���ο� ������ ��ȯ
	m_pAction = m_pActionList[iOutput].get();
}
void TGame::FrameState(TObject* pGame)
{
	m_pAction->m_pOwner = this;
	m_pAction->ProcessAction(pGame);
}
void		TGame::Init() {

}
void		TGame::Frame() {
	m_pAction->Frame();
}
void		TGame::Render() {
	m_pAction->Render();
}
void		TGame::Release()
{
	for (auto data : m_pActionList)
	{
		data->Release();
	}
}