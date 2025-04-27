#include "TItem.h"
#include "THeroObj.h"

void TItem::HitOverlap(Object* pObj, HitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();
	if (OtherType == ObjectType::Hero)
	{
		auto pHero = dynamic_cast<THeroObj*>(pObj);
		pHero->m_HeroHP += 10;
		m_bDead = true;
	}
}
void TItem::Frame()
{
	SetVertexData();

	m_fCurrentTime += g_fSPF;

	if (m_fCurrentTime >= m_fItemFrameTime)
	{
		m_fCurrentTime -= m_fItemFrameTime;
		m_iItemFrame++; // 다음 프레임으로 이동
		if (m_iItemFrame >= m_rtItemFrames.size())
		{
			m_iItemFrame = 0; // 반복일 경우 첫 프레임으로 이동
		}
	}
}
void TItem::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtItemFrames.resize(SpriteList[9].size());

	m_rtItemFrames = SpriteList[9];

}
void TItem::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	Rect rt;
	rt.SetS(m_rtItemFrames[m_iItemFrame].left,
		m_rtItemFrames[m_iItemFrame].top,
		m_rtItemFrames[m_iItemFrame].right,
		m_rtItemFrames[m_iItemFrame].bottom); // 현재 프레임의 텍스처 좌표,

	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}