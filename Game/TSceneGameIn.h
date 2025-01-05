#pragma once
#include "TScene.h"
class TNextBtn : public TButtonGUI
{
public:
	void    HitSelect(TObject* pObj, THitResult hRet)
	{
		m_iSelectState = hRet.iState;
		if (m_iSelectState == T_SELECTED)
		{
			m_pWorld->m_pScene->m_bNextScene = true;
		}
	};
};
class TPrevBtn : public TButtonGUI
{
public:
	void    HitSelect(TObject* pObj, THitResult hRet)
	{
		m_iSelectState = hRet.iState;
		if (m_iSelectState == T_SELECTED)
		{
			m_pWorld->m_pScene->m_bPrevScene = true;
		}
	};
};

using RECT_ARRAY = std::vector<RECT>;

class TSceneGameIn : public TScene
{
	TFiniteStateMachine      m_fsm;
	TGUIFSM					 m_GuiFSM;
	std::shared_ptr<TWorld>  m_pWorld;
	TVector2        m_vCamera = { 640.0f, 400.0f };
	TTexture* m_pBitmap1Mask = nullptr;
	using tObject = std::shared_ptr<TObject>;
	using tNpc = std::shared_ptr<TNpcObj>;
	using tUI = std::shared_ptr<TControlGUI>;
	using tCol = std::shared_ptr<TCollisionManager>;

	std::shared_ptr<TMapObj>	m_pMap = nullptr;
	std::shared_ptr<THeroObj>	m_pHero = nullptr;
	std::vector<tCol>			m_ColList;
	std::vector<tNpc>		    m_NpcList;
	std::vector< tUI>		    m_UiList;
	std::list<tObject>			m_EffectList;
	std::vector<std::pair<TVector2, TVector2>> rectArea;
	UINT						m_Npccount = 1;
	float						m_offsetdis = 5.0f;

	TSound* m_pSound = nullptr;
	TSound* m_pJumpSound = nullptr;
	TSound* m_pShotSound = nullptr;
	TSound* m_pCrashSound = nullptr;

	std::vector<RECT_ARRAY>  m_rtSpriteList;
	std::vector<T_STR_VECTOR> m_szSpriteList;
public:
	void   AddEffect(TVector2 tStart, TVector2 tEnd);
	bool   GameDataLoad(W_STR filename);

	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
public:
	virtual void ProcessAction(TObject* pObj);
public:
	TSceneGameIn(TGame* p);
	TSceneGameIn() { m_iState = TSceneState::SCENE_INGAME; };
	virtual ~TSceneGameIn();
public:
	bool CreateRect();
	bool CreateSound();
	bool CreateMap();
	bool CreateHero();
	bool CreateNPC();
	bool CreateEffect();
	bool CreateUI();
	TVector2 GetWorldMousePos();
};

