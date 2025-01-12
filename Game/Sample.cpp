#include "Sample.h"

void   Sample::Init()
{
	TGame::CreateActionFSM();
	m_SceneFSM.AddStateTransition(SCENE_INTRO, EVENT_NEXT_SCENE, SCENE_LOBBY);
	m_SceneFSM.AddStateTransition(SCENE_LOBBY, EVENT_NEXT_SCENE, SCENE_INGAME);
	m_SceneFSM.AddStateTransition(SCENE_INGAME, EVENT_NEXT_SCENE, SCENE_RESULT);
	m_SceneFSM.AddStateTransition(SCENE_INGAME, EVENT_PREV_SCENE, SCENE_LOBBY);
	m_SceneFSM.AddStateTransition(SCENE_RESULT, EVENT_NEXT_SCENE, SCENE_LOBBY);
	m_SceneFSM.AddStateTransition(SCENE_RESULT, EVENT_PREV_SCENE, SCENE_INGAME);
	m_SceneFSM.AddStateTransition(SCENE_INGAME, EVENT_BOSS_DIED, SCENE_FINAL);
	m_Game.SetFSM(&m_SceneFSM);
	m_Game.Init();
}
void   Sample::Frame()
{
	m_Game.FrameState(nullptr);
	m_Game.Frame();
};
void   Sample::Render()
{
	m_Game.Render();
	/*D2D1_RECT_F rt = { 0.0f, 350.0f, 800.0f, 600.0f };
	m_DxWrite.DirectDraw(rt, L"TSceneGameIn::Render");*/
}
void   Sample::Release()
{
	m_Game.Release();
}

//GameStart(800, 600);
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	Sample sample;
	sample.SetWindowClass(hInstance);
	sample.SetWindow(L"test", 1280, 800);
	sample.GameRun();
}