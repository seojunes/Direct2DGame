#include "TDropObj.h"
#include "THeroObj.h"

void    TDropObj::HitOverlap(Object* pObj, HitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();

	if (OtherType == ObjectType::Wall)
	{
		m_bExplosion = true;
		//m_state = DropState::STATE_EXPLOSION;
		//m_bDead = true;
	}
	else if (OtherType == ObjectType::Hero)
	{
		auto pHero = dynamic_cast<THeroObj*>(pObj);
		pHero->TakeDamage(30);
		if(pHero->m_bInvincible == false)	m_bExplosion = true;
		//m_state = DropState::STATE_EXPLOSION;
		//m_bDead = true;
	}

}

void TDropObj::SetVertexData()
{
	Rect rt;
	switch (m_state)
	{
	case DropState::STATE_DROP:
		rt.SetS(m_rtDropFrame[m_iDropFrame].left,
			m_rtDropFrame[m_iDropFrame].top,
			m_rtDropFrame[m_iDropFrame].right,
			m_rtDropFrame[m_iDropFrame].bottom);
		break;
	case DropState::STATE_EXPLOSION:
		rt.SetS(m_rtExplosion[m_iExplosion].left,
			m_rtExplosion[m_iExplosion].top,
			m_rtExplosion[m_iExplosion].right,
			m_rtExplosion[m_iExplosion].bottom);
		SetScale(75.0f, 100.0f);
		
		break;
	case DropState::STATE_DEAD:
		break;
	default:
		break;
	}
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };

}


void TDropObj::Frame()
{

	SetVertexData();
	switch (m_state)
	{
	case DropState::STATE_DROP:
		m_vDir = { 0.0f, 1.0f };
		m_fSpeed = 300.0f;
		m_vPos = m_vPos + m_vDir * g_fSPF * m_fSpeed;
		SetPosition(m_vPos);
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		if (m_fCurrentTime >= m_fDropFrameTime)
		{
			m_fCurrentTime -= m_fDropFrameTime;
			m_iDropFrame++; // 다음 프레임으로 이동
			if (m_iDropFrame >= m_rtDropFrame.size())
			{
				m_iDropFrame = 0; 
			}
		}
		if (m_bExplosion == true)
		{
			m_state = DropState::STATE_EXPLOSION;
			AddPosition(0.0f, -50.0f);
		}
		break;
	case DropState::STATE_EXPLOSION:
		m_vDir = { 0.0f, 0.0f };
		m_fCurrentTime += g_fSPF; // 시간 업데이트
		m_fExplosionTime -= g_fSPF;
		if (m_fCurrentTime >= m_fExplosionFrameTime)
		{
			m_fCurrentTime -= m_fExplosionFrameTime;
			m_iExplosion++; // 다음 프레임으로 이동
			if (m_iExplosion >= m_rtExplosion.size())
			{
				m_iExplosion = m_rtExplosion.size() - 1; // 마지막 프레임 유지
			}
		}
		if (m_fExplosionTime < 0.0f)
		{
			m_state = DropState::STATE_DEAD;
		}
		break;
	case DropState::STATE_DEAD:
		m_bDead = true;
		break;
	default:
		break;
	}
}

void TDropObj::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtDropFrame.resize(SpriteList[19].size());
	m_rtExplosion.resize(SpriteList[20].size());

	m_rtDropFrame = SpriteList[19];
	m_rtExplosion = SpriteList[20];
}