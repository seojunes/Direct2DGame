#include "TPortal.h"
#include "TWorld.h"
void    TPortal::HitOverlap(Object* pObj, HitResult hRes)
{
	Object::HitOverlap(pObj, hRes);
	const ObjectType OtherType = pObj == nullptr ? ObjectType::None : pObj->GetType();
	if (OtherType == ObjectType::Hero)
	{
		m_eActive = PortalActiveState::STATE_ACTIVE;
		m_bAble = true;
	}
	else
	{
		m_eActive = PortalActiveState::STATE_INACTIVE;
		m_bAble = false;
	}

}

void TPortal::SetVertexData()
{
	if (m_pTexture == nullptr) return;
	float xSize = m_pTexture->m_TexDesc.Width;
	float ySize = m_pTexture->m_TexDesc.Height;
	Rect rt;
	switch (m_eActive)
	{
	case PortalActiveState::STATE_ACTIVE:
		rt.SetS(m_rtAPortalFrames[m_iPortalFrame].left,
			m_rtAPortalFrames[m_iPortalFrame].top,
			m_rtAPortalFrames[m_iPortalFrame].right,
			m_rtAPortalFrames[m_iPortalFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	case PortalActiveState::STATE_INACTIVE:
		rt.SetS(m_rtPortalFrames[m_iPortalFrame].left,
			m_rtPortalFrames[m_iPortalFrame].top,
			m_rtPortalFrames[m_iPortalFrame].right,
			m_rtPortalFrames[m_iPortalFrame].bottom); // 현재 프레임의 텍스처 좌표,
		break;
	default:
		break;
	}
	m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
	m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
	m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
	m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
}

void TPortal::Frame()
{
	Object2D::Frame();
	SetVertexData();
	m_fCurrentTime += g_fSPF; // 시간 업데이트
	/*switch (m_eActive)
	{*/
	//case PortalActiveState::STATE_ACTIVE:
	//	if (m_fCurrentTime >= m_fPortalFrameTime)
	//	{
	//		m_fCurrentTime -= m_fPortalFrameTime;
	//		m_iAPortalFrame++; // 다음 프레임으로 이동
	//		if (m_iAPortalFrame >= m_rtAPortalFrames.size())
	//		{
	//			if (m_bLoop)
	//				m_iAPortalFrame = 0; // 반복일 경우 첫 프레임으로 이동
	//			else
	//				m_iAPortalFrame = m_rtAPortalFrames.size() - 1; // 마지막 프레임 유지
	//		}
	//	}
	//	break;
	/*case PortalActiveState::STATE_INACTIVE:*/
		if (m_fCurrentTime >= m_fPortalFrameTime)
		{
			m_fCurrentTime -= m_fPortalFrameTime;
			m_iPortalFrame++; // 다음 프레임으로 이동
			if (m_iPortalFrame >= m_rtPortalFrames.size())
			{
				if (m_bLoop)
					m_iPortalFrame = 0; // 반복일 경우 첫 프레임으로 이동
				else
					m_iPortalFrame = m_rtPortalFrames.size() - 1; // 마지막 프레임 유지
			}
		}
	/*	break;
	default:
		break;
	}*/
	
	
}

void TPortal::SetData(vector<vector<RECT>> SpriteList)
{
	m_rtPortalFrames.resize(SpriteList[9].size());
	m_rtAPortalFrames.resize(SpriteList[10].size());

	m_rtPortalFrames = SpriteList[9];
	m_rtAPortalFrames = SpriteList[10];
}