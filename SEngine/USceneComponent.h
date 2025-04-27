#pragma once
#include "UActorComponent.h"
#include "Matrix.h"
class USceneComponent : public UActorComponent
{
public:
	Matrix		m_matWorld;
public:
	virtual void   Init() {};
	virtual void   Tick() {};
	virtual void   Render() {};
	virtual void   Destroy() {};
};

