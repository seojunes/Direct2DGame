#include "TMissileFSM.h"
#include "TMissile.h"

TMissileState::TMissileState(TMissileObj* p) : m_pOwner(p) {

}
TMissileState::~TMissileState() {}

TIdleAction::TIdleAction(TMissileObj* p) : TMissileState(p) {
	m_iMissileState = 0;
}
TIdleAction::~TIdleAction() {}
TShotAction::TShotAction(TMissileObj* p) : TMissileState(p) {
	m_iMissileState = 1;
}
TShotAction::~TShotAction() {}

TFlyingAction::TFlyingAction(TMissileObj* p) : TMissileState(p) {
	m_iMissileState = 2;
}
TFlyingAction::~TFlyingAction() {}

THitAction::THitAction(TMissileObj* p) : TMissileState(p) {
	m_iMissileState = 3;
}
THitAction::~THitAction() {}


TExplodeAction::TExplodeAction(TMissileObj* p) : TMissileState(p) {
	m_iMissileState = 4;
}
TExplodeAction::~TExplodeAction() {}
TDestroyAction::TDestroyAction(TMissileObj* p) : TMissileState(p) {
	m_iMissileState = 5;
}
TDestroyAction::~TDestroyAction() {}

void TIdleAction::ProcessAction(TObject* pObj)
{
	m_pOwner->SetPosition(pObj->m_vPos);
}
void TShotAction::ProcessAction(TObject* pObj)
{
	m_pOwner->m_vPos = m_pOwner->m_vPos + m_pOwner->m_vDir * (g_fSPF * m_pOwner->m_fSpeed);
	m_pOwner->SetPosition(m_pOwner->m_vPos);
}
void TFlyingAction::ProcessAction(TObject* pObj)
{

}

void THitAction::ProcessAction(TObject* pObj)
{

}
void TExplodeAction::ProcessAction(TObject* pObj)
{

}
void TDestroyAction::ProcessAction(TObject* pObj)
{

}