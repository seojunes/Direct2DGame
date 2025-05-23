#pragma once
#include "SEngine.h"
#include "SFbximporter.h"
class Sample : public SEngine
{
	SFbxImporter	m_FbxImporter;
	std::vector<std::shared_ptr<AActor>>   m_FbxObjs;
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

