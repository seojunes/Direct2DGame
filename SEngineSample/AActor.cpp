#include "AActor.h"
void AActor::Init() {
	if (Mesh != nullptr)Mesh->Init();
}
void AActor::Tick()
{
	if (Mesh != nullptr) Mesh->Tick();
}
void AActor::Render() {
	if (Mesh != nullptr) Mesh->Render();
}
void AActor::Destroy() {
	if (Mesh != nullptr) Mesh->Destroy();
}