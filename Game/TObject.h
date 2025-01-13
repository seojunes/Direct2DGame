#pragma once
#include "TCollision.h"
#include "TMeshRender.h"
class TWorld;

enum class TObjectType
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

class TObject
{
public:
	TMeshRender* m_pMeshRender = nullptr;
	UINT			m_iCollisionID;
	UINT			m_iSelectID;
	UINT			m_iSelectState = 0;
	TWorld* m_pWorld = nullptr;
	TCollisionType	m_iCollisionType = TCollisionType::T_Ignore;
public:
	TMatrix3   m_matScale;
	TMatrix3   m_matRotate;
	TMatrix3   m_matTrans;
	TMatrix3   m_matWorld; // s * r * t
	TVector2   m_vScale = { 1.0f, 1.0f };
	float      m_fAngleRadian = 0.0f;
	TVector2   m_vCamera;
	TVector2		m_vPos;
	TVector2		m_vDir;
	float			m_fSpeed;
	float			m_fAlpha = 0.0f;
	bool			m_bDead = false;
	TRect			m_rtScreen;
	TSphere			m_Sphere;
	TLoadResData	m_LoadResData;
	TShader* m_pShader = nullptr;
	TTexture* m_pTexture = nullptr;
	std::vector<TVector2>		m_vScreenList;
	std::vector<PCT_VERTEX>		m_vVertexList;
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
	virtual TObject& SetShader(TShader* pShader = nullptr);
	virtual TObject& SetTexture(TTexture*);
	virtual TObject& SetLayout(TInputLayout* pInputLayout = nullptr);
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
	virtual void    HitOverlap(TObject* pObj, THitResult hRet);
	virtual void    HitSelect(TObject* pObj, THitResult hRet);
public:
	virtual TObjectType GetType() const 
	{
		return TObjectType::None; // 기본값
	}
public:
	TObject();
	virtual ~TObject();
};
