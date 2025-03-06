#pragma once
#include "SEngine.h"

class Sample : public SEngine
{
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};



