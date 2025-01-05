//#pragma once
//#include "TMapObj.h"
//#include "TFiniteState.h"
//class THeroObj;
//class THeroState
//{
//public:
//	UINT m_iHeroState;
//	THeroState(THeroObj* p);
//	THeroState() {};
//	virtual ~THeroState();
//public:
//	virtual void ProcessAction(TObject* pObj) = 0;
//public:
//	THeroObj* m_pOwner;
//};
//
//
////enum TCollisionState
////{
////	T_UPPER_COLLISION = 0,   // 머리 충돌
////	T_LOWER_COLLISION,       // 바닥 충돌
////	T_LEFT_COLLISION,        // 왼쪽 충돌
////	T_RIGHT_COLLISION,       // 오른쪽 충돌
////	T_COLCOUNTER,
////};
////
////enum TCollisionEvent
////{
////	EVENT_UPPER = 0,
////	EVENT_UPPER,
////	EVENT_UPPER,
////	EVENT_UPPER,
////	EVENT_COL_COUNT,
////};
//
//class TIdleAction : public THeroState
//{
//public:
//	virtual void ProcessAction(TObject* pObj);
//public:
//	TIdleAction(THeroObj* p);
//	TIdleAction() { m_iHeroState = T_IDLE; };
//	virtual ~TIdleAction();
//};
//
//
//class TUpperAction : public THeroState
//{
//public:
//	virtual void ProcessAction(TObject* pObj);
//public:
//	TUpperAction(THeroObj* p);
//	TUpperAction() { m_iHeroState = T_UPPER_COLLISION; };
//	virtual ~TUpperAction();
//};
//
//class TLowerAction : public THeroState
//{
//public:
//	virtual void ProcessAction(TObject* pObj);
//public:
//	TLowerAction(THeroObj* p);
//	TLowerAction() { m_iHeroState = T_LOWER_COLLISION; };
//	virtual ~TLowerAction();
//};
//
//class TRightAction : public THeroState
//{
//public:
//	virtual void ProcessAction(TObject* pObj);
//public:
//	TRightAction(THeroObj* p);
//	TRightAction() { m_iHeroState = T_RIGHT_COLLISION; };
//	virtual ~TRightAction();
//};
//
//class TLeftAction : public THeroState
//{
//public:
//	virtual void ProcessAction(TObject* pObj);
//public:
//	TLeftAction(THeroObj* p);
//	TLeftAction() { m_iHeroState = T_LEFT_COLLISION; };
//	virtual ~TLeftAction();
//};
//
//
//
//
