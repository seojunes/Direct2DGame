#pragma once
#include "AActor.h"
class SSkyObject : public AActor
{
	std::vector < std::shared_ptr<UMaterial>> m_Materials;
public:
	bool Load(shared_ptr <UStaticMeshComponent> sm);
	void Render() override;
	void PostRender() override;
};

