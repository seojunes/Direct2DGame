#include "TEnemyFSM.h"
#include "TNpcObj.h"
TEnemyState::TEnemyState(TNpcObj* p) : m_pOwner(p) {

}
TEnemyState::~TEnemyState() {}
TStandAction::TStandAction(TNpcObj* p) : TEnemyState(p) {
	m_iState = 0;
}
TStandAction::~TStandAction() {}
TMoveAction::TMoveAction(TNpcObj* p) : TEnemyState(p) {
	m_iState = 1;
}
TMoveAction::~TMoveAction() {}

TAttackAction::TAttackAction(TNpcObj* p) : TEnemyState(p) {
	m_iState = 2;
}
TAttackAction::~TAttackAction() {}

void TStandAction::ProcessAction(TObject* pObj)
{
	m_pOwner->m_StateData[m_iState].m_fTimer -= g_fSPF;
	if (m_pOwner->m_StateData[m_iState].m_fTimer < 0.0f)
	{
		m_pOwner->m_StateData[m_iState].m_fTimer =
			m_pOwner->m_StateData[m_iState].m_fDefaultTimer;
		m_pOwner->SetTransition(TActionEvent::EVENT_PATROL);
	}
	m_pOwner->SetRotation(0);
}
void TMoveAction::ProcessAction(TObject* pObj)
{
	// 공격범위 판단 -> 상태전이
	// 고격범위 탈출 -> 상태전이
	float fDistance = (pObj->m_vPos - m_pOwner->m_vPos).Length();
	if (fDistance < m_pOwner->m_StateData[m_iState].m_fDistance)
	{
		m_pOwner->m_StateData[m_iState].m_fTimer =
			m_pOwner->m_StateData[m_iState].m_fDefaultTimer;
		m_pOwner->SetTransition(TActionEvent::EVENT_FINDTARGET);
		return;
	}
	// v = v + d*s : 직선의 벡터의 방정식
	TVector2 vMove = m_pOwner->m_vPos +
		m_pOwner->m_vDir * (g_fSPF * m_pOwner->m_fSpeed);
	m_pOwner->SetPosition(vMove);
	m_pOwner->SetRotation(T_Pi);
}
void TAttackAction::ProcessAction(TObject* pObj)
{
	// 공격범위 판단 -> 상태전이
	// 고격범위 탈출 -> 상태전이
	m_pOwner->m_StateData[m_iState].m_fTimer -= g_fSPF;
	if (m_pOwner->m_StateData[m_iState].m_fTimer < 0.0f)
	{
		m_pOwner->m_StateData[m_iState].m_fTimer =
			m_pOwner->m_StateData[m_iState].m_fDefaultTimer;
		m_pOwner->SetTransition(TActionEvent::EVENT_STOP);
		return;
	}
	float fDistance = (pObj->m_vPos - m_pOwner->m_vPos).Length();
	if (fDistance > m_pOwner->m_StateData[m_iState].m_fDistance)
	{
		m_pOwner->m_StateData[m_iState].m_fTimer =
			m_pOwner->m_StateData[m_iState].m_fDefaultTimer;
		m_pOwner->SetTransition(TActionEvent::EVENT_LOSTTARGET);
		return;
	}

	TVector2 vDir = (pObj->m_vPos - m_pOwner->m_vPos);
	m_pOwner->m_vDir = vDir.Normal();
	TVector2 vMove = m_pOwner->m_vPos +
		m_pOwner->m_vDir * (g_fSPF * m_pOwner->m_fSpeed);
	m_pOwner->SetPosition(vMove);
	m_pOwner->SetRotation(g_fGT);
}