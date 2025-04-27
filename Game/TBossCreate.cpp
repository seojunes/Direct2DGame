#include "TBossCreate.h"
void    TBossCreate::HitOverlap(Object* pObj, HitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();

	if (OtherType == ObjectType::Wall)
	{
		m_bCreate = true;
	}

}

void TBossCreate::Frame()
{
	if (!m_bGameState)		return;

	if (m_vPos.y <= 843.5f)	m_vPos.y += 500.0f * g_fSPF;
	SetPosition(m_vPos);	
}

void TBossCreate::SetVertexData()
{

	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	Rect rt;
	rt.SetP(22.0f, 48.0f, 32.0f, 111.0f);
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };

}

void TBossCreate::GetState(bool state)
{
	m_bGameState = state;
}