#pragma once
#include "Object2D.h"
struct EffectData
{
	std::vector<RECT>   m_rtList;
	std::vector<T_STR>   m_szList;
	UINT			    m_iNumAnimFrame = 0;
	float				m_fLifeTime = 1.0f;
	float				m_fOffsetTime = 1.0f;
	bool			    m_bLoop = false;
	UINT				m_iType = 0;
	TVector2			m_vDirection = { 1.0f, 0.0f };
	UINT				m_iDamage = 5;
	Object* m_pOwner = nullptr;
};
class EffectObj : public Object2D
{
public:
	Texture* m_pCurrentTexture = nullptr;
	EffectData		m_Data;// 초기데이터
public:
	std::vector<RECT>   m_rtList;
	UINT			m_iNumAnimFrame = 0;
	UINT			m_iAnimFrame = 0;
	float			m_fLifeTime = 1.0f;
	float			m_fOffsetTime = 1.0f;
	bool			m_bLoop = false;
	bool			m_bDead = false;
	float			m_fCurrentTime = 0.0f;
public:
	virtual void SetData(EffectData data);
	virtual void Frame() override;
	virtual void Render() override;
	virtual void SetVertexData();
};

class TEffectObjSingle : public EffectObj {};
class TEffectObjUV : public EffectObj {};
class TEffectObjTex : public EffectObj {};