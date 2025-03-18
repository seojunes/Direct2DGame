#pragma once
#include "SEngine.h"
#include "AActor.h"
class Sample : public SEngine
{
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

