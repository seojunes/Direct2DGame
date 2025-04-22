#include "GuiFSM.h"
#include "ControlGUI.h"
GuiState::GuiState(ControlGUI* p) : m_pOwner(p) {

}
GuiState::~GuiState() {}

DefaultActionGui::DefaultActionGui(ControlGUI* p) : GuiState(p) {
	m_iState = SelectState::T_DEFAULT;
}
DefaultActionGui::~DefaultActionGui() {}

HoverActionGui::HoverActionGui(ControlGUI* p) : GuiState(p) {
	m_iState = SelectState::T_HOVER;
}
HoverActionGui::~HoverActionGui() {}

ActiveActionGui::ActiveActionGui(ControlGUI* p) : GuiState(p) {
	m_iState = SelectState::T_ACTIVE;
}
ActiveActionGui::~ActiveActionGui() {}

SelectedActionGui::SelectedActionGui(ControlGUI* p) : GuiState(p) {
	m_iState = SelectState::T_SELECTED;
}
SelectedActionGui::~SelectedActionGui() {}

void DefaultActionGui::ProcessAction(Object* pObj)
{
	if (m_pOwner->m_iSelectState == SelectState::T_HOVER)
	{
		m_pOwner->SetTransition(SelectEvent::EVENT_SELECT);
		return;
	}
}
void HoverActionGui::ProcessAction(Object* pObj)
{
	if (m_pOwner->m_iSelectState == SelectState::T_DEFAULT)
	{
		m_pOwner->SetScale(m_pOwner->m_StateData[SelectEvent::EVENT_DEFAULT].vDefaultScale);
		m_pOwner->SetTransition(SelectEvent::EVENT_DEFAULT);
		return;
	}
	if (m_pOwner->m_iSelectState == SelectState::T_SELECTED)
	{
		m_pOwner->SetScale(m_pOwner->m_StateData[SelectEvent::EVENT_DEFAULT].vDefaultScale);
		m_pOwner->SetTransition(SelectEvent::EVENT_SELECT);
		return;
	}
	float fAngle = m_pOwner->m_StateData[SelectEvent::EVENT_SELECT].
		vDefaultScale.y + 5.0f;
	m_pOwner->SetScale(m_pOwner->m_vScale.x, fAngle);
}
void ActiveActionGui::ProcessAction(Object* pObj)
{
	if (m_pOwner->m_iSelectState == SelectState::T_ACTIVE)
	{
		m_pOwner->SetTransition(SelectEvent::EVENT_SELECT);
		return;
	}
}
void SelectedActionGui::ProcessAction(Object* pObj)
{
}


