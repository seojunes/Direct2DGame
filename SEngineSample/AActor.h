#pragma once
#include "UObject.h"
#include "TMatrix.h"
#include "UStaticMeshComponent.h"
// AActor.h �� ȭ�鿡 ��ġ ������ ������Ʈ
struct cbData
{
	TMatrix matWorld;
	TMatrix matView;
	TMatrix matProj;

};
class AActor : public UObject
{
public:
	cbData m_cbData;
	UStaticMeshComponent* Mesh;
	ComPtr<ID3D11Buffer> m_pConstantBuffer = nullptr;
public:
	virtual bool CreateConstantBuffer();
	void SetMesh(UStaticMeshComponent* mesh);

public:
	virtual void Init();
	virtual void Tick();
	virtual void Render();
	virtual void Destroy();

public:
	AActor();
	virtual ~AActor();
};

