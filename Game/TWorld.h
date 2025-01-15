#pragma once
#include "TMapObj.h"
#include "THeroObj.h"
#include "TEffectObj.h"
#include "TNpcObj.h"
#include "TControlGUI.h"
#include "TProjectile.h"
#include "TCollisionManager.h"
#include "TMonster.h"
#include "TPortal.h"
#include "TBossObj.h"
#include "THpBar.h"
#include "TUiHpBar.h"
#include "TVictory.h"
#include "TRadderObj.h"
#include "TBossCreate.h"


class TScene;
using CollisionFunction = std::function<void(TObject*, THitResult)>;
class TWorld
{
public:
	UINT   m_iExecuteCollisionID = 0;
	std::map<int, TObject*>  m_CollisionList;
	UINT   m_iExecuteSelectID = 0;
	std::map<int, TObject*>  m_SelectList;
	TScene* m_pScene = nullptr;



	std::map<int, TObjectType> m_ObjectCollect;

public:
	std::map<int, CollisionFunction>  m_fnCollisionExecute;
	void AddCollisionExecute(TObject* pOwnder, CollisionFunction fun);
	void DeleteCollisionExecute(TObject* pOwner);

	std::map<int, CollisionFunction>  m_fnSelectExecute;
	void AddSelectExecute(TObject* pOwnder, CollisionFunction fun);
	void DeleteSelectExecute(TObject* pOwner);
public:
	void   Frame();
	void   Release();
public:
	TWorld(TScene* pScene);
};

