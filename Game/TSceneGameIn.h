#pragma once
#include "TScene.h"

enum class Debug
{
	Normal = 0,
	Debug,
};

enum class MapAction
{
	STATE_STAGE = 0,
	STATE_CREATEBOSS,
	STATE_BOSS,
};
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
	using tHP = std::shared_ptr<THpBar>;
	using tDrop = std::shared_ptr<TDropObj>;

	std::shared_ptr<TMapObj>	m_pMap = nullptr;
	std::shared_ptr<TMapObj>	m_pBossMap = nullptr;
	std::shared_ptr<TPortal>	m_pPortal = nullptr;
	std::shared_ptr<TVictory>	m_pVictory = nullptr;
	std::shared_ptr<TRadderObj>	m_pRadder = nullptr;
	std::shared_ptr<THeroObj>	m_pHero = nullptr;
	std::shared_ptr<TBossObj>	m_pBoss = nullptr;
	std::shared_ptr<TBossCreate>	m_pBossCreate = nullptr;
	std::vector<tDrop>			m_DropList;
	std::vector<tCol>			m_ColList;
	std::vector<tNpc>		    m_NpcList;
	std::vector< tUI>		    m_UiList;
	std::vector<tHP>		    m_HPList;
	std::list<tObject>			m_EffectList;
	std::vector<std::pair<TVector2, TVector2>> rectArea;
	std::vector<std::pair<TVector2, TVector2>> DropArea;
	std::vector<std::pair<TVector2, TVector2>> Mon1Area;
	std::vector<std::pair<TVector2, TVector2>> Mon2Area;
	std::vector<std::pair<TVector2, TVector2>> Mon3Area;
	std::vector<std::pair<TVector2, TVector2>> HPArea;
	std::set<int>							   m_iDrop;
	

	UINT						m_Npccount = 4;
	float						m_offsetdis = 5.0f;



	std::vector<RECT_ARRAY>  m_rtSpriteList;
	std::vector<T_STR_VECTOR> m_szSpriteList;
public:
	void   AddEffect(TVector2 tStart, TVector2 tEnd);
	void   AddDrop();
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
	//bool CreateSound() override;
	bool CreateMap();
	bool CreateBoss();
	bool CreateObject();
	bool CreateBossMap();
	bool CreateHero();
	bool CreateNPC();
	//bool CreateEffect();
	bool CreateUI();
	
	TVector2 GetWorldMousePos();
public:
	Debug		m_Debug = Debug::Normal;
	MapAction   m_MapAction = MapAction::STATE_STAGE;
	float		m_fBossInitCount = 1.0f;
	bool		m_bBossDefeated = false;
};

