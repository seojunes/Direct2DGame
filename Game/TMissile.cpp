#include "TMissile.h"
#include "TDevice.h"

std::vector<std::shared_ptr<TMissileState>>  TMissileObj::m_pMActionList;

void    TMissileObj::HitOverlap(TObject* pObj, THitResult hRes)
{
	//m_bDead = true;
}

void TMissileObj::Frame()
{
	TVector2 vAdd = m_vPos;
	//사라지는 조건도 EVENT로 관리 곧 지울예정.
	if (m_vPos.x > m_pHero->m_rtScreen.v2.x + 600 || m_vPos.x < m_pHero->m_rtScreen.v1.x - 600)
	{
  		m_bDead = true;
	}
	//이동도 EVNET로 관리
	m_vPos = vAdd + m_vDir * (g_fSPF * m_fSpeed);
	SetPosition(m_vPos);

}

void TMissileObj::SetDirrection(HeroView view)
{
	if (view == HeroView::RightView)
		m_vDir.x = 1; // 오른쪽
	else if (view == HeroView::LeftView)
		m_vDir.x = -1; // 왼쪽
}

void TMissileObj::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	TObject2D::SetVertexData();
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	TRect rt;
	rt.SetS(0.0f, 19.0f, 14.0f, 29.0f);
	if (m_vDir.x == 1)
	{
 		m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v1.x / xSize,rt.vs.y / ySize };
		m_vVertexList[3].t = { rt.v2.x / xSize,rt.vs.y / ySize };
	}	
	else if (m_vDir.x == -1)
	{
		m_vVertexList[0].t = { rt.v2.x / xSize,rt.v1.y / ySize };
		m_vVertexList[1].t = { rt.v1.x / xSize,rt.v1.y / ySize };
		m_vVertexList[2].t = { rt.v2.x / xSize,rt.vs.y / ySize };
		m_vVertexList[3].t = { rt.v1.x / xSize,rt.vs.y / ySize };
	}
	
}

void TMissileObj::SetFSM(TFiniteStateMachine* pFsm)
{
	//  STATE_IDLE = 0,        // 대기 상태 (발사 대기)
	//	STATE_SHOT,            // 발사 상태
	//	STATE_FLYING,          // 비행 중
	//	STATE_HIT,             // 목표물 타격
	//	STATE_EXPLODE,         // 폭발 상태
	//	STATE_DESTROY,		   // 소멸 상태
	//	STATE_MISSILE_COUNTER,
	m_pMFsm = pFsm;
	m_MStateData.resize(T_MissileState::STATE_MISSILE_COUNTER);
	m_MStateData[T_MissileState::STATE_IDLE].m_fTimer = 3.0f;
	m_MStateData[T_MissileState::STATE_SHOT].m_fTimer = 3.0f;
	m_MStateData[T_MissileState::STATE_FLYING].m_fTimer = 3.0f;
	m_MStateData[T_MissileState::STATE_HIT].m_fTimer = 3.0f;
	m_MStateData[T_MissileState::STATE_EXPLODE].m_fTimer = 3.0f;
	m_MStateData[T_MissileState::STATE_DESTROY].m_fTimer = 3.0f;
	m_pMAction = m_pMActionList[0].get();
}
void TMissileObj::StartFSM()
{
	if (m_pMActionList.size()) return;
	std::shared_ptr<TMissileState> idle = std::make_shared<TIdleAction>();
	std::shared_ptr<TMissileState> shot =	std::make_shared<TShotAction>();
	std::shared_ptr<TMissileState> fly = std::make_shared<TFlyingAction>();
	std::shared_ptr<TMissileState> hit =  std::make_shared<THitAction>();
	std::shared_ptr<TMissileState> explode=   std::make_shared<TExplodeAction>();
	std::shared_ptr<TMissileState> destory = std::make_shared<TDestroyAction>();
	m_pMActionList.emplace_back(idle);
	m_pMActionList.emplace_back(shot);
	m_pMActionList.emplace_back(fly);
	m_pMActionList.emplace_back(hit);
	m_pMActionList.emplace_back(explode);
	m_pMActionList.emplace_back(destory);
}
void TMissileObj::SetTransition(UINT iEvent)
{
	_ASSERT(m_pMFsm);
	UINT iOutput = m_pMFsm->GetOutputState(	m_pMAction->m_iMissileState, iEvent);
	m_pMAction = m_pMActionList[iOutput].get();
}
void TMissileObj::FrameState(TObject* pNpc)
{
	m_pMAction->m_pOwner = this;
	m_pMAction->ProcessAction(pNpc);
}