#include "TProjectile.h"
#include "TGameCore.h"
#include "TWorld.h"


void TProjectileEffect::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
	if (pObj->GetType() == TObjectType::Npc)
	{
		//m_bDead = true; // 충돌 시 미사일 소멸
	}
	if (pObj->GetType() == TObjectType::Wall)
	{
		m_bDead = true;
	}
}
void TProjectileEffect::Frame()
{
	m_fCurrentTime += g_fSPF;
	m_fLifeTime -= g_fSPF;
	if (m_fLifeTime <= 0.0f) m_bDead = true;
	/*if (m_fCurrentTime > m_fOffsetTime)
	{
		m_iAnimFrame++;
		m_fCurrentTime -= m_fOffsetTime;
	}
	m_fTimer += g_fSPF;*/
	
	if (this->m_pOwnerType == Shooter::OWNER_MON3)
	{	
		m_vDir.x = this->m_Data.m_vDirection.x;
		temp += 5.0f * g_fSPF;
		m_vDir.y = -2.0f + temp;
		m_vDir = m_vDir.Normal();
	}
	else
	{
		m_vDir = this->m_Data.m_vDirection;
	}
	m_vPos = m_vPos + m_vDir * 800.0f * g_fSPF;
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

void   TProjectile::AddEffect(TVector2 vStart, TVector2 tEnd, TVector2 direction, Shooter ownertype,TObject* owner, bool  m_bOncharging)
{
	auto obj = std::make_shared<TProjectileEffect>();
	obj->m_pMeshRender = &TGameCore::m_MeshRender;
	obj->m_vVertexList = obj->m_pMeshRender->m_vVertexList;
	obj->m_pOwnerType = ownertype;
	TLoadResData resData;
	resData.texShaderName = L"../../data/shader/Default.txt";
	TEffectData data;
	data.m_bLoop = true;
	data.m_fLifeTime = 1.0f;
	data.m_fOffsetTime = 0.01f;
	if (m_bOncharging == false)
	{
		resData.texPathName = L"../../data/ui/newShot.png";
		data.m_iDamage = 5;
		obj->m_fSpeed = 700.0f;
		//obj->m_iDamage = 5;
	}
	else
	{
		resData.texPathName = L"../../data/ui/newCShot.png";
		data.m_iDamage = 15;
		obj->m_fSpeed = 500.0f;
		//obj->m_iDamage = 15;
	}

	if (ownertype == Shooter::OWNER_MON2)
	{
		resData.texPathName = L"../../data/ui/Mon2Shot.png";
		data.m_iDamage = 30;
	}
	else if (ownertype == Shooter::OWNER_MON3)
	{
		resData.texPathName = L"../../data/ui/bomb.png";
		data.m_iDamage = 10;
		data.m_fLifeTime = 30.0f;
	}
	else if (ownertype == Shooter::OWNER_BOSS1)
	{
		resData.texPathName = L"../../data/ui/bossMissile1.png";
		data.m_iDamage = 10;
		data.m_fLifeTime = 3.0f;
	}
	
	data.m_vDirection = direction;
	UINT iSprite = rand() % 3;
	data.m_iType = 1;// rand() % m_szSpriteList[0].size();
	data.m_iNumAnimFrame = 1;
	data.m_pOwner = owner;
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

	//if (m_pWorld)
	//{ // TWorld 포인터가 유효한지 확인
	//	m_pWorld->AddCollisionExecute(obj.get(), std::bind(&TProjectile::HitOverlap, this, std::placeholders::_1, std::placeholders::_2));
	//}
	if (obj->Create(m_pWorld, resData, vStart, tEnd))
	{
		obj->m_pCurrentTexture = obj->m_pTexture;
		obj->m_iCollisionType = TCollisionType::T_Overlap;
		m_datalist.emplace_back(obj);
	}

}

//void TProjectile::ApplyOwnerType(const TObjectType& InObjectType)
//{
//	OwnerType = InObjectType;
//}

void   TProjectile::Init()
{

}
void   TProjectile::Frame(TVector2 vObjPos)
{
	for (auto iter = std::begin(m_datalist); iter != m_datalist.end();)
	{
		TProjectileEffect* pObj = (TProjectileEffect*)iter->get();
		if (pObj->m_bDead == false)
		{
			pObj->m_vObjPos = vObjPos;
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