#pragma once
#include "SEngine.h"
#include "AActor.h"
class Sample : public SEngine
{
	std::shared_ptr<TCamera> m_pGameCamera;
	std::shared_ptr<AActor> m_SMHero;
	std::shared_ptr<AActor> m_SMBox;
	std::shared_ptr<AActor> m_SMPlane;
	std::shared_ptr<AActor> m_SMLine;
	std::vector< std::shared_ptr<UMaterial>> m_Materials;
public:
	UStaticMeshComponent* Load(std::wstring filename);
	std::map<std::wstring, std::shared_ptr<UStaticMeshComponent>>  m_Shapes;
public:
	UStaticMeshComponent* GetShape(std::wstring name);
	void CreateDefaultShapes();
	void CreateBoxShapes();
	void CreatePlaneShapes();
	void CreateLineShapes();
public:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Destroy() override;
};

