#pragma once
#include "UActorComponent.h"
#include "TMatrix.h"
class USceneComponent : public UActorComponent
{
public:
	TMatrix		m_matWorld;
public:
	virtual void   Init() {};
	virtual void   Tick() {};
	virtual void   Render() {};
	virtual void   Destroy() {};
};

