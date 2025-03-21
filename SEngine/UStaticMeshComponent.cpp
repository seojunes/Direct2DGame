#include "UStaticMeshComponent.h"
#include "TDevice.h"

void   UStaticMeshComponent::Init()
{

}
void   UStaticMeshComponent::Tick()
{
};
void   UStaticMeshComponent::Destroy()
{
};

void	UStaticMeshComponent::Render()
{
	PreRender();
	PostRender();
	for (auto child : m_Childs)
	{
		child->Render();
	}
}