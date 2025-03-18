#pragma once
#include "SEngine.h"
#include "SFbximporter.h"
class Sample : public SEngine
{
	SFbxImporter	m_FbxImporter;
	AActor			m_FbxBox;
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

