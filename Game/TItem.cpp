#include "TItem.h"
void TItem::HitOverlap(TObject* pObj, THitResult hRes)
{
	TObject::HitOverlap(pObj, hRes);
	const TObjectType OtherType = pObj == nullptr ? TObjectType::None : pObj->GetType();
}
void TItem::Frame()
{

}
void TItem::SetData(vector<vector<RECT>> SpriteList)
{

}
void TItem::SetVertexData()
{

}