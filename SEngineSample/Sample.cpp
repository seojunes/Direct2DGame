#include "Sample.h"
void Sample::Init()
{

}
void Sample::Tick() {

}
void Sample::Render() {

}
void Sample::Destroy() {

}

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	Sample sample;
	sample.SetWindowClass(hInstance);
	sample.SetWindow(L"test", 800, 600);
	sample.GameRun();
}