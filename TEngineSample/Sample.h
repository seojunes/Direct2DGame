#pragma once
#include "TEngine.h"
class Sample : public TEngine
{
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

