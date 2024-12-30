#include "TNpcObj.h"
#include "TDevice.h"
struct TCollisionInfo
{

};
TEnemyState::TEnemyState(TNpcObj* p) : m_pOwner(p) {

}
TEnemyState::~TEnemyState() {}
TStandAction::TStandAction(TNpcObj* p) : TEnemyState(p) {
	m_iEnemyState = 0;
}
TStandAction::~TStandAction() {}
TMoveAction::TMoveAction(TNpcObj* p) : TEnemyState(p) {
	m_iEnemyState = 1;
}
TMoveAction::~TMoveAction() {}

TAttackAction::TAttackAction(TNpcObj* p) : TEnemyState(p) {
	m_iEnemyState = 2;
}
TAttackAction::~TAttackAction() {}

void TStandAction::ProcessAction(TObject* pObj)
{
	m_fTimer += g_fSPF;
	if (m_fTimer > 3.0f)
	{
		m_fTimer = 0.0f;
		m_pOwner->SetTransition(T_ActionEvent::EVENT_PATROL);
	}
	m_pOwner->SetRotation(0);
}
void TMoveAction::ProcessAction(TObject* pObj)
{
	// 공격범위 판단 -> 상태전이
	// 고격범위 탈출 -> 상태전이
	float fDistance = (pObj->m_vPos - m_pOwner->m_vPos).Length();
	if (fDistance < 100.0f)
	{
		m_fTimer = 0.0f;
		m_pOwner->SetTransition(T_ActionEvent::EVENT_FINDTARGET);
		return;
	}
	/*m_pOwner->SetTransition(T_ActionEvent::EVENT_LOSTTARGET);
	return;

	m_fTimer += g_fSPF;
	if (m_fTimer > 5.0f)
	{
		m_fTimer = 0.0f;
		m_pOwner->SetTransition(T_ActionEvent::EVENT_STOP);
		return;
	}	*/
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
	m_fTimer += g_fSPF;
	if (m_fTimer > 10.0f)
	{
		m_fTimer = 0.0f;
		m_pOwner->SetTransition(T_ActionEvent::EVENT_STOP);
		return;
	}
	float fDistance = (pObj->m_vPos - m_pOwner->m_vPos).Length();
	if (fDistance > 100.0f)
	{
		m_fTimer = 0.0f;
		m_pOwner->SetTransition(T_ActionEvent::EVENT_LOSTTARGET);
		return;
	}

	TVector2 vDir = (pObj->m_vPos - m_pOwner->m_vPos);
	m_pOwner->m_vDir = vDir.Normal();
	TVector2 vMove = m_pOwner->m_vPos +
		m_pOwner->m_vDir * (g_fSPF * m_pOwner->m_fSpeed);
	m_pOwner->SetPosition(vMove);
	m_pOwner->SetRotation(g_fGT);
}
void    TNpcObj::HitOverlap(TObject* pObj, THitResult hRet)
{
	//m_bDead = true;
};
void TNpcObj::Frame()
{
	if (m_vPos.x > m_pMap->m_rtScreen.v2.x - m_rtScreen.vh.x)
	{
		m_vDir.x *= -1.0f;
		m_vPos.x = m_pMap->m_rtScreen.v2.x - m_rtScreen.vh.x;
	}
	if (m_vPos.x < m_pMap->m_rtScreen.v1.x + m_rtScreen.vh.x)
	{
		m_vDir.x *= -1.0f;
		m_vPos.x = m_pMap->m_rtScreen.v1.x + m_rtScreen.vh.x;
	}
	if (m_vPos.y > m_pMap->m_rtScreen.v2.y - m_rtScreen.vh.y)
	{
		m_vDir.y *= -1.0f;
		m_vPos.y = m_pMap->m_rtScreen.v2.y - m_rtScreen.vh.y;
	}
	if (m_vPos.y < m_pMap->m_rtScreen.v1.y + m_rtScreen.vh.y)
	{
		m_vDir.y *= -1.0f;
		m_vPos.y = m_pMap->m_rtScreen.v1.y + m_rtScreen.vh.y;
	}
}

void TNpcObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	TObject2D::SetVertexData();
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(46.0f, 62.0f, 114.0f, 142.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}