#pragma once
#include "TGuiFSM.h"

struct TStateDataUI
{
	float m_fTimer;
	float m_fDefaultTimer;
	float m_fDistance;
	TVector2 vDefaultScale;
	TVector2 vDefaultPos;
	TVector2 vDefaultRotate;
};

class TGUIFSM : public TFiniteStateMachine
{

};

class TControlGUI : public TObject2D
{
public:
	TGuiState* m_pAction = nullptr;
	std::vector<TStateDataUI>   m_StateData;
	static std::vector<std::shared_ptr<TGuiState>>  m_pActionList;
	TFiniteStateMachine* m_pFsm = nullptr;
public:
	virtual void SetFSM(TFiniteStateMachine* pFsm);
	virtual void SetTransition(UINT iEvent);
	virtual void FrameState(TObject* pHero);
public:
	virtual void Frame() override;
	void	Transform(TVector2 vCamera)override;
	void    TransformNDC()override;
	void	SetScale(TVector2 s)override;
	void	SetScale(float sx, float sy)override;
	void	SetVertexData()override;
	void	SetIndexData()override;
	bool    LoadTexture(std::wstring texName)override;
	void    HitSelect(TObject* pObj, THitResult hRet) override;
	bool	Create(TWorld* pWorld, TLoadResData data, TVector2 s, TVector2 e) override;
	TControlGUI()
	{
		m_fSpeed = 150.0f;
		m_vDir.x = 1.0f;
		m_vDir.y = 1.0f;
	}
};
class TButtonGUI : public TControlGUI
{
public:
	std::vector<TTexture*> m_pTexState;
	static void CreateActionFSM();
public:
	void SetFSM(TFiniteStateMachine* pFsm)override;
	void SetTransition(UINT iEvent)override;
	void Frame()override;
	void PostRender()override;
	bool LoadTexture(std::wstring texName)override;
};
class TImageGUI : public TButtonGUI
{
public:
	void PostRender()override;
	bool LoadTexture(std::wstring texName)override;
};

