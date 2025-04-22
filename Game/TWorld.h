#pragma once
#include "TMapObj.h"
#include "THeroObj.h"
#include "EffectObj.h"
#include "TNpcObj.h"
#include "ControlGUI.h"
#include "TProjectile.h"
#include "TCollisionManager.h"
#include "TMonster.h"
#include "TPortal.h"
#include "TBossObj.h"
#include "HpBar.h"
#include "UiHpBar.h"
#include "TVictory.h"
#include "TRadderObj.h"
#include "TBossCreate.h"
#include "TDropObj.h"
#include "TItem.h"


class TScene;
using CollisionFunction = std::function<void(Object*, THitResult)>;
class TWorld
{
public:
	UINT   m_iExecuteCollisionID = 0;
	std::map<int, Object*>  m_CollisionList;
	UINT   m_iExecuteSelectID = 0;
	std::map<int, Object*>  m_SelectList;
	TScene* m_pScene = nullptr;



	std::map<int, ObjectType> m_ObjectCollect;

public:
	std::map<int, CollisionFunction>  m_fnCollisionExecute;
	void AddCollisionExecute(Object* pOwnder, CollisionFunction fun);
	void DeleteCollisionExecute(Object* pOwner);

	std::map<int, CollisionFunction>  m_fnSelectExecute;
	void AddSelectExecute(Object* pOwnder, CollisionFunction fun);
	void DeleteSelectExecute(Object* pOwner);
public:
	void   Frame();
	void   Release();
public:
	TWorld(TScene* pScene);
};

