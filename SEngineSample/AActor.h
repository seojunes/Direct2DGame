#pragma once
#include "UObject.h"
#include "UStaticMeshComponent.h"
// AActor.h �� ȭ�鿡 ��ġ ������ ������Ʈ
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

