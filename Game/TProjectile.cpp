#include "TProjectile.h"
#include "TGameCore.h"
#include "TWorld.h"
void TProjectileEffect::Frame()
{
	m_fCurrentTime += g_fSPF;
	m_fLifeTime -= g_fSPF;
	if (m_fLifeTime <= 0.0f) m_bDead = true;
	if (m_fCurrentTime > m_fOffsetTime)
	{
		m_iAnimFrame++;
		m_fCurrentTime -= m_fOffsetTime;
	}
	m_fTimer += g_fSPF;
	m_vDir = { 0.0f, -1.0f };
	m_vPos = m_vHeroPos + m_vDir * 700.0f * m_fTimer;
	m_vPos.x = m_vHeroPos.x;
	SetPosition(m_vPos);

	/*if (m_Data.m_iType == 0)
	{
		if (m_iAnimFrame >= m_rtList.size())
		{
			m_iAnimFrame = 0;
			if (m_bLoop == false)
			{
				m_bDead = true;
			}
		}
	}
	if (m_Data.m_iType == 1)
	{
		if (m_iAnimFrame >= m_Data.m_szList.size())
		{
			m_iAnimFrame = 0;
			if (m_bLoop == false)
			{
				m_bDead = true;
			}
		}
	}*/


	/*if (m_Data.m_iType == 0)
	{
		TRect rt;
		rt.SetS(m_rtList[m_iAnimFrame].left,
			m_rtList[m_iAnimFrame].top,
			m_rtList[m_iAnimFrame].right,
			m_rtList[m_iAnimFrame].bottom);

		if (m_pTexture)
		{
			float xSize = m_pTexture->m_TexDesc.Width;
			float ySize = m_pTexture->m_TexDesc.Height;
			m_vVertexList[0].t = { rt.v1.x / xSize,rt.v1.y / ySize };
			m_vVertexList[1].t = { rt.v2.x / xSize,rt.v1.y / ySize };
			m_vVertexList[2].t = { rt.v1.x / xSize,rt.v2.y / ySize };
			m_vVertexList[3].t = { rt.v2.x / xSize,rt.v2.y / ySize };
		}
	}
	if (m_Data.m_iType == 1)
	{
		m_pCurrentTexture = I_Tex.GetPtr(m_Data.m_szList[m_iAnimFrame]);
	}*/
}
void TProjectileEffect::Render()
{
	//// 텍스처 교체 스프라이트
	//if (m_Data.m_iType == 1)
	//{
	PreRender();
	TDevice::m_pd3dContext->PSSetShaderResources(0, 1, &m_pCurrentTexture->m_pTexSRV);
	if (m_pShader)
	{
		TDevice::m_pd3dContext->VSSetShader(m_pShader->m_pVertexShader.Get(), nullptr, 0);
		TDevice::m_pd3dContext->PSSetShader(m_pShader->m_pPixelShader.Get(), nullptr, 0);
	}
	PostRender();
}

TProjectile::TProjectile(TWorld* pWorld) : m_pWorld(pWorld)
{

}
void   TProjectile::AddEffect(TVector2 vStart, TVector2 tEnd)
{
	auto obj = std::make_shared<TProjectileEffect>();
	obj->m_pMeshRender = &TGameCore::m_MeshRender;
	obj->m_vVertexList = obj->m_pMeshRender->m_vVertexList;
	TLoadResData resData;
	resData.texPathName = L"../../data/ui/8.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	TEffectData data;
	data.m_bLoop = true;
	data.m_fLifeTime = 1.0f;
	data.m_fOffsetTime = 0.01f;
	UINT iSprite = rand() % 3;
	data.m_iType = 1;// rand() % m_szSpriteList[0].size();
	data.m_iNumAnimFrame = 1;
	/*if (data.m_iType == 0)
	{
		data.m_iNumAnimFrame = m_rtSpriteList[iSprite].size();
		data.m_rtList = m_rtSpriteList[iSprite];
	}
	if (data.m_iType == 1)
	{
		data.m_iNumAnimFrame = m_szSpriteList[0].size();
		data.m_szList = m_szSpriteList[0];
	}*/
	obj->SetData(data);
	if (obj->Create(m_pWorld, resData, vStart, tEnd))
	{
		obj->m_pCurrentTexture = obj->m_pTexture;
		m_datalist.emplace_back(obj);
	}
}
void   TProjectile::Init()
{

}
void   TProjectile::Frame(TVector2 vHeroPos)
{
	for (auto iter = std::begin(m_datalist); iter != m_datalist.end();)
	{
		TProjectileEffect* pObj = (TProjectileEffect*)iter->get();
		if (pObj->m_bDead == false)
		{
			pObj->m_vHeroPos = vHeroPos;
			pObj->Frame();
			iter++;
		}
		else
		{
			m_pWorld->DeleteCollisionExecute(pObj);
			pObj->Release();
			iter = m_datalist.erase(iter);
		}
	}
}
void   TProjectile::Render(TVector2 vCamera)
{
	for (auto data : m_datalist)
	{
		data->Transform(vCamera);
		data->Render();
	}

}
void   TProjectile::Release() {

}