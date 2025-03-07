#pragma once
#include "SEngine.h"
#include "AActor.h"
class Sample : public SEngine
{
	std::shared_ptr<AActor> m_pActor;
	UStaticMeshComponent* Load(std::wstring filename);
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

