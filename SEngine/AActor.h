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
	TVector4 vData;
};
struct cbAnimData
{
	TMatrix matBone[250];
};

class AActor : public UObject
{
public:
	std::vector<TMatrix>	m_CurrentAnimMatrix;
	USceneComponent* RootComponent;
	//UInputComponent* InputComponent;
	//std::list<UActorComponent*> m_pReplicatedComponents;
public:
	TVector3 m_vLook = { 0, 0, 1 };
	TVector3 m_vRight = { 1, 0, 0 };
	TVector3 m_vUp = { 0, 1, 0 };
	TMatrix m_matOffset;
	TMatrix m_matParent;
	TMatrix m_matScale;
	TMatrix m_matRotation;
	TMatrix m_matTrans;
	TMatrix m_matWorld;
	cbData  m_cbData;
	cbAnimData m_cbAnimData;
	UINT    m_iStartFrame;
	UINT    m_iEndFrame;
	float   m_fFrame = 0.0f;				// 시간계산을 해주기 위해서 실수값으로 초기화 
	bool    m_bInc = true;
	shared_ptr<UStaticMeshComponent> Mesh;
	ComPtr<ID3D11Buffer>  m_pConstantBuffer = nullptr;
	ComPtr<ID3D11Buffer>  m_pCurrentAnimationCB = nullptr;
public:
	TVector3 m_vScale;
	TVector3 m_vRotation;// pitch, yaw, roll
	TVector3 m_vPosition;
	UStaticMeshComponent* GetMesh() { return Mesh.get(); }
public:
	virtual bool	CreateConstantBuffer();
	void SetMesh(std::shared_ptr<UStaticMeshComponent> mesh);
	void UpdateVector();
public:
	virtual void Init();
	virtual void Tick();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual void Destroy();
public:
	AActor();
	virtual ~AActor();
};

