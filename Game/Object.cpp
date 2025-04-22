#include "Object.h"
#include "Device.h"
#include "TWorld.h"
void    Object::HitOverlap(Object* pObj, THitResult hRet)
{
	if (pObj == nullptr)
	{
		return;
	}
	auto Type = pObj->GetType();

	if (Type != ObjectType::Projectile && GetType() != ObjectType::Projectile)
	{
		return;
	}

  	switch (Type)
	{
	case ObjectType::None:
		break;
	case ObjectType::Hero:
		break;
	case ObjectType::Projectile:
		break;
	case ObjectType::Npc:
		break;
	case ObjectType::Wall:
		break;
	default:
		break;
	}
};
void    Object::HitSelect(Object* pObj, THitResult hRet)
{
}
void    Object::FadeIn(float fAlpha)
{
	for (auto& data : m_vVertexList)
	{
		data.c.w = m_fAlpha + fAlpha;
	}
}
void    Object::FadeOut(float fAlpha)
{
	for (auto& data : m_vVertexList)
	{
		data.c.w = m_fAlpha - fAlpha;
	}
}
void    Object::Fade()
{
	for (auto& data : m_vVertexList)
	{
		data.c.w = m_fAlpha;
	}
}

void Object::SetScale(float sx, float sy)
{
	m_vScale.x = sx;
	m_vScale.y = sy;
	m_matScale.Scale(m_vScale);
	//m_rtScreen.vc = m_vPos;
	m_rtScreen.Size(m_vScale * 2.0f);
	m_Sphere.vCenter = m_rtScreen.vc;
	m_Sphere.fRadius = m_rtScreen.fR;
}
void Object::SetScale(TVector2 s)
{
	SetScale(s.x, s.y);
}
void Object::SetRotation(float fRadian)
{
	m_fAngleRadian = fRadian;
	m_matRotate.Rotate(m_fAngleRadian);
}
void Object::SetPosition(TVector2 p)
{
	m_vPos = p;
	m_matTrans.Trans(m_vPos);
	m_rtScreen.Move(m_vPos.x, m_vPos.y);
	m_Sphere.vCenter = m_rtScreen.vc;
	m_Sphere.fRadius = m_rtScreen.fR;
}
void Object::AddPosition(float x, float y)
{
	m_vPos.x += x;
	m_vPos.y += y;
	m_rtScreen.Move(m_vPos.x, m_vPos.y);
	m_Sphere.vCenter = m_rtScreen.vc;
	m_Sphere.fRadius = m_rtScreen.fR;
	m_matTrans.Trans(m_vPos);
}
void Object::AddPosition(TVector2 v)
{
	AddPosition(v.x, v.y);
}
void Object::AddScale(float x, float y)
{
	m_vScale.x += x;
	m_vScale.y += y;
	m_matScale.Scale(m_vScale);
}
void Object::AddScale(TVector2 v)
{
	m_vScale.x += v.x;
	m_vScale.y += v.y;
	m_matScale.Scale(m_vScale);
}
void Object::AddRotation(float angle)
{
	m_fAngleRadian += angle;
	m_matRotate.Rotate(m_fAngleRadian);
}
void	Object::Init()
{}
void	Object::Frame()
{}
void	Object::Transform(TVector2 vCamera)
{
	m_vCamera = vCamera;
	m_matWorld =  m_matRotate * m_matScale * m_matTrans;
	for (int i = 0; i < m_vScreenList.size(); i++)
	{
		m_vScreenList[i] =
			m_pMeshRender->m_vScreenList[i] *
			m_matWorld;
	}
}
void	Object::PreRender()
{
	m_pMeshRender->PreRender();
}
void	Object::Render()
{
	PreRender();
	if (m_pTexture)
	{
		Device::m_pd3dContext->PSSetShaderResources(
			0, 1, &m_pTexture->m_pTexSRV);
	}

	if (m_pShader)
	{
		Device::m_pd3dContext->VSSetShader(
			m_pShader->m_pVertexShader.Get(), nullptr, 0);
		Device::m_pd3dContext->PSSetShader(
			m_pShader->m_pPixelShader.Get(), nullptr, 0);
	}
	PostRender();
}
void	Object::PostRender()
{
	m_pMeshRender->PostRender();
}
void	Object::Release()
{

}
bool	Object::CreateVertexShader()
{
	if (m_LoadResData.texShaderName.empty())
	{
		return true;
	}
	m_pShader = I_Shader.Load(m_LoadResData.texShaderName);
	if (m_pShader == nullptr)
	{
		return false;
	}
	return true;
}
bool	Object::CreatePixelShader()
{
	if (m_LoadResData.texShaderName.empty())
	{
		return true;
	}
	if (m_pShader && m_pShader->m_pPixelShader == nullptr)
	{
		/*m_pShader = I_Shader.Load(m_LoadResData.texShaderName);
		if (m_pShader == nullptr)
		{
			return false;
		}*/
		return false;
	}
	return true;
}
bool	Object::Create(TWorld* pWorld)
{
	m_pWorld = pWorld;
	Init();
	SetVertexData();
	/*if (!CreateVertexBuffer())
	{
		return false;
	}
	SetIndexData();
	if (!CreateIndexBuffer())
	{
		return false;
	}*/
	if (!CreateVertexShader())
	{
		return false;
	}
	if (!CreatePixelShader())
	{
		return false;
	}
	/*if (!CreateInputLayout())
	{
		return false;
	}*/

	auto bindFun = std::bind(&Object::HitOverlap, this,std::placeholders::_1,	std::placeholders::_2);
	if(m_bCollisionCheck == true)	m_pWorld->AddCollisionExecute(this,	bindFun);
	m_bCollisionCheck = true;
	return true;
}
bool	Object::Create(TWorld* pWorld, TLoadResData data)
{
	m_LoadResData = data;
	if (!LoadTexture(m_LoadResData.texPathName))
	{
		return false;
	}
	return Create(pWorld);
}
bool	Object::Create(TWorld* pWorld, TLoadResData data,
	TVector2 s,
	TVector2 e)
{
	m_LoadResData = data;
	m_rtScreen.SetP(s, e);
	SetScale(m_rtScreen.vh.x, m_rtScreen.vh.y);
	SetRotation(m_fAngleRadian);
	SetPosition(m_rtScreen.vc);

	m_vVertexList.resize(4);
	m_vScreenList.resize(4);

	if (m_pMeshRender)
	{
		m_vVertexList = m_pMeshRender->m_vVertexList;
	}

	if (!LoadTexture(m_LoadResData.texPathName))
	{
		return false;
	}
	return Create(pWorld);
}

Object& Object::SetShader(Shader* pData)
{
	if (pData == nullptr)
	{
		m_pMeshRender->m_pShader = I_Shader.g_pDefaultShader;
	}
	else
	{
		m_pMeshRender->m_pShader = pData;
	}
	return *this;
}
Object& Object::SetTexture(Texture* pData)
{
	m_pMeshRender->m_pTexture = pData;
	return *this;
}
Object& Object::SetLayout(InputLayout* pData)
{
	if (pData == nullptr)
	{
		m_pMeshRender->m_pInputLayout = I_InputLayout.g_pInputLayout;
	}
	else
	{
		m_pMeshRender->m_pInputLayout = pData;
	}
	return *this;
}
bool   Object::LoadTexture(std::wstring texName)
{
	m_pTexture = I_Tex.Load(texName);
	if (m_pTexture == nullptr)
	{
		return false;
	}
	return true;
}


Object::Object()
{
	m_rtScreen.SetS(0.0f, 0.0f, (float)g_ptClientSize.x, (float)g_ptClientSize.y);
	m_vPos.x = 0.0f;
	m_vPos.y = 0.0f;
	m_fSpeed = 100.0f;
}
Object::~Object()
{
}
