#pragma once
#include "Collision.h"
#include "MeshRender.h"
class TWorld;

enum class ObjectType
{
	None,
	Hero,
	Projectile,
	Npc,
	Wall,
	Portal,
	Radder,
	Boss,
};

class Object
{
public:
	MeshRender* m_pMeshRender = nullptr;
	UINT			m_iCollisionID;
	UINT			m_iSelectID;
	UINT			m_iSelectState = 0;
	TWorld* m_pWorld = nullptr;
	CollisionType	m_iCollisionType = CollisionType::T_Ignore;
public:
	Matrix3   m_matScale;
	Matrix3   m_matRotate;
	Matrix3   m_matTrans;
	Matrix3   m_matWorld; // s * r * t
	TVector2   m_vScale = { 1.0f, 1.0f };
	float      m_fAngleRadian = 0.0f;
	TVector2   m_vCamera;
	TVector2		m_vPos;
	TVector2		m_vDir;
	float			m_fSpeed;
	float			m_fAlpha = 0.0f;
	bool			m_bDead = false;
	Rect			m_rtScreen;
	Sphere			m_Sphere;
	TLoadResData	m_LoadResData;
	Shader* m_pShader = nullptr;
	Texture* m_pTexture = nullptr;
	std::vector<TVector2>		m_vScreenList;
	std::vector<PCT_VERTEX>		m_vVertexList;

	bool m_bCollisionCheck = true;
public:
	virtual void	FadeIn(float fAlpha);
	virtual void	FadeOut(float fAlpha);
	virtual void	Fade();
public:
	virtual void SetScale(float sx, float sy);
	virtual void SetScale(TVector2 s);
	virtual void SetRotation(float fRadian);
	virtual void SetPosition(TVector2 p);
	virtual void AddPosition(float x, float y);
	virtual void AddPosition(TVector2 v);
	virtual void AddScale(float x, float y);
	virtual void AddScale(TVector2 v);
	virtual void AddRotation(float angle);
public:
	virtual bool	Create(TWorld* pWorld);
	virtual bool	Create(TWorld* pWorld, TLoadResData data);
	virtual bool	Create(TWorld* pWorld, TLoadResData data,
		TVector2 s,		// 화면좌표 시작
		TVector2 t);   // 화면좌표 끝		
	virtual Object& SetShader(Shader* pShader = nullptr);
	virtual Object& SetTexture(Texture*);
	virtual Object& SetLayout(InputLayout* pInputLayout = nullptr);
	virtual bool LoadTexture(std::wstring texName);
	virtual bool	CreateVertexShader();
	virtual bool	CreatePixelShader();
	virtual void    SetVertexData() {};
	virtual void    SetIndexData() {};
public:
	virtual void	Init();
	virtual void	Frame();
	virtual void    Transform(TVector2 vCamera);
	virtual void	PreRender();
	virtual void	Render();
	virtual void	PostRender();
	virtual void	Release();
public:
	virtual void    HitOverlap(Object* pObj, HitResult hRet);
	virtual void    HitSelect(Object* pObj, HitResult hRet);
public:
	virtual ObjectType GetType() const 
	{
		return ObjectType::None; // 기본값
	}
public:
	Object();
	virtual ~Object();
};
