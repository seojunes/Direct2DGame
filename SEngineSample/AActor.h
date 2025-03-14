#pragma once
#include "UObject.h"
#include "TMatrix.h"
#include "UStaticMeshComponent.h"
// AActor.h 는 화면에 배치 가능한 오브젝트

struct cbData
{
	TMatrix  matWorld;
	TMatrix  matView;
	TMatrix  matProj;
};
class AActor : public UObject
{
public:
	USceneComponent* RootComponent;
	//UInputComponent* InputComponent;
	std::list<UActorComponent*> m_pReplicatedComponents;
public:
	TVector3 m_vLook;
	TVector3 m_vRight;
	TMatrix m_matOffset;
	TMatrix m_matParent;
	TMatrix m_matScale;
	TMatrix m_matRotation;
	TMatrix m_matTrans;
	TMatrix m_matWorld;
	cbData  m_cbData;
	UStaticMeshComponent* Mesh;
	ComPtr<ID3D11Buffer>  m_pConstantBuffer = nullptr;
public:
	TVector3 m_vScale;
	TVector3 m_vRotation;// pitch, yaw, roll
	TVector3 m_vPosition;
	UStaticMeshComponent* GetMesh() { return Mesh; }
public:
	virtual bool	CreateConstantBuffer();
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

