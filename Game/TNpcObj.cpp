#include "TNpcObj.h"
#include "TDevice.h"
#include "THeroObj.h"
struct TCollisionInfo
{

};
std::vector<std::shared_ptr<TEnemyState>> TNpcObj::m_pActionList;

void TNpcObj::HitOverlap(TObject* pObj, THitResult hRes)  //충돌했을떄 실행되는 콜백함수
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();
	//if (OtherType == TObjectType::Hero)			 //충돌체가 Hero일때, 
	//{
	//	// Hero와 충돌 시 Hero의 체력을 감소
	//	auto pHero = dynamic_cast<THeroObj*>(pObj);
	//	if (pHero && pHero->m_bInvincible == false)
	//	{
	//		pHero->TakeDamage(5);
	//	}
	//}
	if (OtherType == TObjectType::Projectile)
	{
		auto pMissile = dynamic_cast<TProjectileEffect*>(pObj);
		if (pMissile && pMissile->m_pOwnerType == Shooter::OWNER_HERO)
		{
			
			pMissile->m_bDead = true;
			m_HP -= pMissile->m_Data.m_iDamage;
			if (m_HP <= 0)		m_HP = 0;
			m_bAttacked = true;
			//m_eAttacked = ATTACKSTATE::STATE_OK;
		}
	}
	if (OtherType == TObjectType::Wall)
	{
		auto pWall = dynamic_cast<TCollisionManager*>(pObj);
		if (pWall)
		{
			if (m_vPos.y > pWall->m_rtScreen.v1.y)
			{
				if (m_vPos.x < pWall->m_rtScreen.v1.x)
				{
					m_vPos.x = pWall->m_rtScreen.v1.x - m_rtScreen.vh.x;
				}
				else
				{
					m_vPos.x = pWall->m_rtScreen.v2.x + m_rtScreen.vh.x;
				}
			}
			
		}
	}
}

TVector2 TNpcObj::Attacked(TVector2 pos)
{
	m_bAttacked = false;
	return pos;
}
void TNpcObj::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtBossCreateFrames.resize(SpriteList[12].size());
	m_rtBossIdleFrames.resize(SpriteList[13].size());
	m_rtBossAttack1Frames.resize(SpriteList[14].size());
	m_rtBossTransFrames.resize(SpriteList[15].size());
	m_rtBoss2PhaseFrames.resize(SpriteList[16].size());
	m_rtBossM1Frames.resize(SpriteList[17].size());
	m_rtBossM2Frames.resize(SpriteList[18].size());
	m_rtBossFlyingFrames.resize(SpriteList[21].size());
	m_rtBossDyingFrames.resize(SpriteList[22].size());
	
	m_rtBossCreateFrames = SpriteList[12];
	m_rtBossIdleFrames = SpriteList[13];
	m_rtBossAttack1Frames = SpriteList[14];
	m_rtBossTransFrames = SpriteList[15];
	m_rtBoss2PhaseFrames = SpriteList[16];
	m_rtBossM1Frames = SpriteList[17];
	m_rtBossM2Frames = SpriteList[18];
	m_rtBossFlyingFrames = SpriteList[21];
	m_rtBossDyingFrames = SpriteList[22];


	m_rtMon1AttackFrames.resize(SpriteList[8].size());
	m_rtMon2AttackFrames.resize(SpriteList[23].size());
	m_rtMon2DyingFrames.resize(SpriteList[24].size());
	m_rtMon3AttackFrames.resize(SpriteList[25].size());
	m_rtMon1AttackFrames = SpriteList[8];
	m_rtMon2AttackFrames = SpriteList[23];
	m_rtMon2DyingFrames = SpriteList[24];
	m_rtMon3AttackFrames = SpriteList[25];
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
	std::shared_ptr<TEnemyState> stand = std::make_shared<TStandAction>();
	std::shared_ptr<TEnemyState> move = std::make_shared<TMoveAction>();
	std::shared_ptr<TEnemyState> attack = std::make_shared<TAttackAction>();
	std::shared_ptr<TEnemyState> dead = std::make_shared<TDeadAction>();
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