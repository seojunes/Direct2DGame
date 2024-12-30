#pragma once
#include "TMapObj.h"
#include "THeroObj.h"
#include "TEffectObj.h"
#include "TMissile.h"
#include "TNpcObj.h"
//void fun(object, UINT);
//std::function<void ()>
using CollisionFunction = std::function<void(TObject*, THitResult)>;
class TWorld
{
public:
	UINT   m_iExecuteCollisionID = 0;
	std::map<int, TObject*>  m_ObjectList;
public:
	std::map<int, CollisionFunction>  m_fnCollisionExecute;
	void AddCollisionExecute(TObject* pOwnder,
		CollisionFunction fun);
	void DeleteCollisionExecute(TObject* pOwner);
public:
	void   Frame();
};

