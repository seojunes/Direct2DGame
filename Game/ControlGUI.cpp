#include "ControlGUI.h"
#include "Device.h"
#include "TWorld.h"

std::vector<std::shared_ptr<GuiState>> ControlGUI::m_pActionList;

void    ControlGUI::HitSelect(Object* pObj, THitResult hRet)
{
	m_iSelectState = hRet.iState;
	if (m_iSelectState == T_SELECTED)
	{
		m_iSelectState = T_SELECTED;
	}
};
bool	ControlGUI::Create(TWorld* pWorld, TLoadResData data, TVector2 s, TVector2 e)
{
	m_pWorld = pWorld;
	m_LoadResData = data;
	m_rtScreen.SetP(s, e);
	SetScale(m_rtScreen.vh.x, m_rtScreen.vh.y);
	SetRotation(m_fAngleRadian);
	SetPosition(m_rtScreen.vc);

	if (!LoadTexture(m_LoadResData.texPathName))
	{
		return false;
	}

	m_StateData[SelectEvent::EVENT_DEFAULT].vDefaultScale = m_vScale;
	m_StateData[SelectEvent::EVENT_SELECT].vDefaultScale = m_vScale;

	auto bindFun = std::bind(&Object::HitSelect,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_pWorld->AddSelectExecute(this, bindFun);
	return Object2D::Create(pWorld);
}
bool   ControlGUI::LoadTexture(std::wstring texName)
{
	m_pTexture = I_Tex.Load(texName);
	if (m_pTexture == nullptr)
	{
		return false;
	}
	return true;
}
void ControlGUI::Frame()
{
}
void ControlGUI::SetVertexData()
{
	m_vScreenList.resize(4);
	m_vVertexList.resize(4);
	if (m_pMeshRender)
	{
		m_vVertexList = m_pMeshRender->m_vVertexList;
	}
}
void ControlGUI::SetIndexData()
{
	Object2D::SetIndexData();
}
void ControlGUI::Transform(TVector2 vCamera)
{
	Object::Transform(vCamera);
	TransformNDC();
	Device::m_pd3dContext->UpdateSubresource(
		m_pMeshRender->m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
}
void ControlGUI::SetScale(float sx, float sy)
{
	Object2D::SetScale(sx, sy);
	m_vPos = m_rtScreen.vc;
	m_matTrans.Trans(m_vPos);
}
void ControlGUI::SetScale(TVector2 s)
{
	SetScale(s.x, s.y);
}
void ControlGUI::TransformNDC()
{
	for (UINT i = 0; i < m_vVertexList.size(); i++)
	{
		TVector2 ndc = ScreenToNDC(m_vScreenList[i].x, m_vScreenList[i].y, g_ptClientSize);
		m_vVertexList[i].v = ndc;
	}
}
void ControlGUI::SetFSM(FiniteStateMachine* pFsm)
{
}
void ControlGUI::SetTransition(UINT iEvent)
{
}
void ControlGUI::FrameState(Object* pHero)
{
	m_pAction->m_pOwner = this;
	m_pAction->ProcessAction(pHero);
}

void ButtonGUI::SetFSM(FiniteStateMachine* pFsm)
{
	m_pFsm = pFsm;
	m_StateData.resize(SelectState::T_COUNTER);
	m_pAction = m_pActionList[0].get();
}
void ButtonGUI::CreateActionFSM()
{
	if (m_pActionList.size()) return;
	std::shared_ptr<GuiState> defaultBtn = std::make_shared<DefaultActionGui>();
	std::shared_ptr<GuiState> HoverBtn = std::make_shared<HoverActionGui>();
	std::shared_ptr<GuiState> ActiveBtn = std::make_shared<ActiveActionGui>();
	std::shared_ptr<GuiState> SelBtn = std::make_shared<SelectedActionGui>();
	m_pActionList.emplace_back(defaultBtn);
	m_pActionList.emplace_back(HoverBtn);
	m_pActionList.emplace_back(ActiveBtn);
	m_pActionList.emplace_back(SelBtn);
}
void ButtonGUI::SetTransition(UINT iEvent)
{
	_ASSERT(m_pFsm);
	UINT iOutput = m_pFsm->GetOutputState(m_pAction->m_iState, iEvent);
	m_pAction = m_pActionList[iOutput].get();
}
void ButtonGUI::Frame()
{
}
void ButtonGUI::PostRender()
{
	Device::m_pd3dContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTexSRV);// &m_pTexState[m_iSelectState]->m_pTexSRV);
	m_pMeshRender->PostRender();
}
bool ButtonGUI::LoadTexture(std::wstring texName)
{
	auto tex0 = I_Tex.Load(texName);
	//auto tex0 = I_Tex.Load(L"../../data/ui/pressStart.png");
	auto tex1 = I_Tex.Load(L"../../data/ui/main_start_sel.png");
	auto tex2 = I_Tex.Load(L"../../data/ui/main_start_pus.png");
	auto tex3 = I_Tex.Load(L"../../data/ui/main_start_dis.png");
	m_pTexture = tex0;

	m_pTexState.emplace_back(m_pTexture);
	m_pTexState.emplace_back(m_pTexture);
	m_pTexState.emplace_back(m_pTexture);
	m_pTexState.emplace_back(m_pTexture);
	return true;
}

void ImageGUI::PostRender()
{
	Device::m_pd3dContext->PSSetShaderResources(0, 1, &m_pTexState[m_iSelectState]->m_pTexSRV);
	m_pMeshRender->PostRender();
}
bool ImageGUI::LoadTexture(std::wstring texName)
{
	auto tex0 = I_Tex.Load(texName);
	//auto tex1 = I_Tex.Load(texName");
	//auto tex2 = I_Tex.Load(texNameg");
	//auto tex3 = I_Tex.Load(texName");
	m_pTexture = tex0;

	m_pTexState.emplace_back(m_pTexture);
	m_pTexState.emplace_back(m_pTexture);
	m_pTexState.emplace_back(m_pTexture);
	m_pTexState.emplace_back(m_pTexture);
	return true;
}