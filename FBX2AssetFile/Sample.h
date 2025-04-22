#pragma once
#include "SEngine.h"
#include "TFbxImporter.h"
//https://aps.autodesk.com/developer/overview/fbx-sdk
class Sample : public SEngine
{
	TFbxImporter   m_FbxImporter;
	std::vector<std::shared_ptr<AAsset>>   m_FbxObjs;
public:
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

