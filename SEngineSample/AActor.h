#pragma once
#include "UObject.h"
#include "UStaticMeshComponent.h"
// AActor.h 는 화면에 배치 가능한 오브젝트
class AActor : public UObject
{
public:
	UStaticMeshComponent* Mesh;
public:
	void SetMesh(UStaticMeshComponent* mesh)
	{
		Mesh = mesh;
	}
public:
	virtual void Init();
	virtual void Tick();
	virtual void Render();
	virtual void Destroy();
};

