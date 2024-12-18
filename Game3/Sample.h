#pragma once
#include "TGameCore.h"
#include "TMapObj.h"
#include "THeroObj.h"
#include "TEffectObj.h"
#include "TSound.h"
using RECT_ARRAY = std::vector<RECT>;
extern int g_iJumpingCount;   // ¸É¹öº¯¼ö·Î ¹Ù²ã¼­ Ã³¸® 
class Sample : public TGameCore
{
	TTexture*		m_pBitmap1Mask = nullptr;
	using tObject = std::shared_ptr<TObject>;
	
	std::shared_ptr<TMapObj>	m_pMap = nullptr;
	std::shared_ptr<THeroObj>	m_pHero = nullptr;

	std::vector<tObject>    m_ObjList;
	std::list<tObject>    m_EffectList;
	TSound* m_pSound = nullptr;
	TSound* m_pSoundEffect = nullptr;
	std::vector<RECT_ARRAY>  m_rtSpriteList;
	std::vector<T_STR_VECTOR> m_szSpriteList;
public:
	void   AddEffect(TVertex2 tStart, TVertex2 tEnd);
	void   AddEffectSingle(TVertex2 tStart, TVertex2 tEnd);
	bool GameDataLoad(W_STR filename);
	virtual void   Init() override;
	virtual void   Frame() override;
	virtual void   Render() override;
	virtual void   Release() override;
};

