#pragma once
#include "GuiFSM.h"

struct StateDataUI
{
	float m_fTimer;
	float m_fDefaultTimer;
	float m_fDistance;
	TVector2 vDefaultScale;
	TVector2 vDefaultPos;
	TVector2 vDefaultRotate;
};

class GUIFSM : public FiniteStateMachine
{

};

class ControlGUI : public Object2D
{
public:
	GuiState* m_pAction = nullptr;
	std::vector<StateDataUI>   m_StateData;
	static std::vector<std::shared_ptr<GuiState>>  m_pActionList;
	FiniteStateMachine* m_pFsm = nullptr;
public:
	virtual void SetFSM(FiniteStateMachine* pFsm);
	virtual void SetTransition(UINT iEvent);
	virtual void FrameState(Object* pHero);
public:
	virtual void Frame() override;
	void	Transform(TVector2 vCamera)override;
	void    TransformNDC()override;
	void	SetScale(TVector2 s)override;
	void	SetScale(float sx, float sy)override;
	void	SetVertexData()override;
	void	SetIndexData()override;
	bool    LoadTexture(std::wstring texName)override;
	void    HitSelect(Object* pObj, THitResult hRet) override;
	bool	Create(TWorld* pWorld, TLoadResData data, TVector2 s, TVector2 e) override;
	ControlGUI()
	{
		m_fSpeed = 150.0f;
		m_vDir.x = 1.0f;
		m_vDir.y = 1.0f;
	}
};
class ButtonGUI : public ControlGUI
{
public:
	std::vector<Texture*> m_pTexState;
	static void CreateActionFSM();
public:
	void SetFSM(FiniteStateMachine* pFsm)override;
	void SetTransition(UINT iEvent)override;
	void Frame()override;
	void PostRender()override;
	bool LoadTexture(std::wstring texName)override;
};
class ImageGUI : public ButtonGUI
{
public:
	void PostRender()override;
	bool LoadTexture(std::wstring texName)override;
};

