#include "TGuiFSM.h"
#include "TControlGUI.h"
TGuiState::TGuiState(TControlGUI* p) : m_pOwner(p) {

}
TGuiState::~TGuiState() {}

TDefaultActionGui::TDefaultActionGui(TControlGUI* p) : TGuiState(p) {
	m_iState = TSelectState::T_DEFAULT;
}
TDefaultActionGui::~TDefaultActionGui() {}

THoverActionGui::THoverActionGui(TControlGUI* p) : TGuiState(p) {
	m_iState = TSelectState::T_HOVER;
}
THoverActionGui::~THoverActionGui() {}

TActiveActionGui::TActiveActionGui(TControlGUI* p) : TGuiState(p) {
	m_iState = TSelectState::T_ACTIVE;
}
TActiveActionGui::~TActiveActionGui() {}

TSelectedActionGui::TSelectedActionGui(TControlGUI* p) : TGuiState(p) {
	m_iState = TSelectState::T_SELECTED;
}
TSelectedActionGui::~TSelectedActionGui() {}

void TDefaultActionGui::ProcessAction(TObject* pObj)
{
	if (m_pOwner->m_iSelectState == TSelectState::T_HOVER)
	{
		m_pOwner->SetTransition(TSelectEvent::EVENT_SELECT);
		return;
	}
}
void THoverActionGui::ProcessAction(TObject* pObj)
{
	if (m_pOwner->m_iSelectState == TSelectState::T_DEFAULT)
	{
		m_pOwner->SetScale(m_pOwner->m_StateData[TSelectEvent::EVENT_DEFAULT].vDefaultScale);
		m_pOwner->SetTransition(TSelectEvent::EVENT_DEFAULT);
		return;
	}
	if (m_pOwner->m_iSelectState == TSelectState::T_SELECTED)
	{
		m_pOwner->SetScale(m_pOwner->m_StateData[TSelectEvent::EVENT_DEFAULT].vDefaultScale);
		m_pOwner->SetTransition(TSelectEvent::EVENT_SELECT);
		return;
	}
	float fAngle = m_pOwner->m_StateData[TSelectEvent::EVENT_SELECT].
		vDefaultScale.y + 5.0f;
	m_pOwner->SetScale(m_pOwner->m_vScale.x, fAngle);
}
void TActiveActionGui::ProcessAction(TObject* pObj)
{
	if (m_pOwner->m_iSelectState == TSelectState::T_ACTIVE)
	{
		m_pOwner->SetTransition(TSelectEvent::EVENT_SELECT);
		return;
	}
}
void TSelectedActionGui::ProcessAction(TObject* pObj)
{
}


