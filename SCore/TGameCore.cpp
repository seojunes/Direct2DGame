#include "TGameCore.h"

void   TGameCore::CoreInit()
{
    Init();
}
void   TGameCore::CoreFrame()
{
    Frame();
}
void   TGameCore::CoreRender()
{
    Render();

}
void   TGameCore::CoreRelease()
{
    Release();
}
bool TGameCore::GameRun()
{
    CoreInit();
    while (m_bRun)
    {
        if (!MessageProcess())
        {
            CoreFrame();
            CoreRender();
        }

    }
    CoreRelease();
    return true;
}