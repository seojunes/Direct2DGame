#pragma once
#include "UMeshComponent.h"
#include "UMaterial.h"
class UStaticMeshComponent : public UMeshComponent
{
	std::shared_ptr<UMaterial> m_pMaterial;
public:
	virtual void  SetMaterial(std::shared_ptr<UMaterial> pMaterial);
public:
	virtual void   Init();
	virtual void   Tick();
	virtual void   PreRender();
	virtual void   Render();
	virtual void   PostRender();
	virtual void   Destroy();
};

