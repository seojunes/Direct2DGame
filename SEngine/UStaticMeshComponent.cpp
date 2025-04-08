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
	/*PreRender();
	PostRender();
	for (auto child : m_Childs)
	{
		auto mesh = child;
		if (mesh != nullptr && mesh->m_AnimList.size() > 0)
		{
			child->m_matWorld =
				TMatrix::Transpose(mesh->m_AnimList[0]);
		}
		child->Render();
	}	*/
}