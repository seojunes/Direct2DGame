#include "TNpcObj.h"
#include "TDevice.h"
#include "THeroObj.h"
struct TCollisionInfo
{

};
std::vector<std::shared_ptr<TEnemyState>> TNpcObj::m_pActionList;

void TNpcObj::HitOverlap(TObject* pObj, THitResult hRes)  //충돌했을떄 실행되는 콜백함수
{
	if (pObj->GetType() == TObjectType::Hero)			 //충돌체가 Hero일때, 
	{
		// Hero와 충돌 시 Hero의 체력을 감소
		auto pHero = dynamic_cast<THeroObj*>(pObj);        
		if (pHero) 
		{
			pHero->m_HP -= 5; // Hero의 HP를 5 감소
			if (pHero->m_HP <= 0) 
			{
				pHero->m_bDead = true; // Hero 사망 처리
			}
		}
	}
}
void TNpcObj::Frame()
{
	/*if (m_vPos.x > m_pMap->m_rtScreen.v2.x - m_rtScreen.vh.x)
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
	}*/
}
void TNpcObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetP(46.0f, 62.0f, 114.0f, 142.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}
void TNpcObj::SetFSM(TFiniteStateMachine* pFsm)
{
	m_pFsm = pFsm;
	m_StateData.resize(TActionState::STATE_COUNT);
	m_StateData[TActionState::STATE_STAND].m_fTimer = 3.0f;
	m_StateData[TActionState::STATE_STAND].m_fDefaultTimer = 3.0f;
	m_StateData[TActionState::STATE_STAND].m_fDistance = 400.0f;
	m_StateData[TActionState::STATE_MOVE].m_fTimer = 3.0f;
	m_StateData[TActionState::STATE_MOVE].m_fDefaultTimer = 3.0f;
	m_StateData[TActionState::STATE_MOVE].m_fDistance = 100.0f;
	m_StateData[TActionState::STATE_ATTACK].m_fTimer = 3.0f;
	m_StateData[TActionState::STATE_ATTACK].m_fDefaultTimer = 3.0f;
	m_StateData[TActionState::STATE_ATTACK].m_fDistance = 100.0f;
	m_pAction = m_pActionList[0].get();
}
void TNpcObj::CreateActionFSM()
{
	if (m_pActionList.size()) return;
	std::shared_ptr<TEnemyState> stand =	std::make_shared<TStandAction>();
	std::shared_ptr<TEnemyState> move =  	std::make_shared<TMoveAction>();
	std::shared_ptr<TEnemyState> attack =	std::make_shared<TAttackAction>();
	std::shared_ptr<TEnemyState> dead =	std::make_shared<TDeadAction>();
	m_pActionList.emplace_back(stand);
	m_pActionList.emplace_back(move);
	m_pActionList.emplace_back(attack);
	m_pActionList.emplace_back(dead);
}
void TNpcObj::SetTransition(UINT iEvent)
{
	_ASSERT(m_pFsm);
	UINT iOutput = m_pFsm->GetOutputState(m_pAction->m_iState, iEvent);
	m_pAction = m_pActionList[iOutput].get();
}
void TNpcObj::FrameState(TObject* pHero)
{
	m_pAction->m_pOwner = this;
	m_pAction->ProcessAction(pHero);
}