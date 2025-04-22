#include "Time.h"
#include <windows.h>
float   g_fSPF =0.0f;
float   g_fGT = 0.0f;
void    Time::Init()
{   
    m_StartClock = system_clock::now();
    m_fTmpTimer = 0.0f;
}
void    Time::Frame()
{
    m_iTmpGameFrame++;
	m_EndClock = system_clock::now();
    duration<float> sec = m_EndClock - m_StartClock;
    float fSPF = sec.count();
    if (fSPF > 0.05f || fSPF < 0.0f)
    {
        fSPF = 0.016f; // 60fps 고정 대체
    }
    g_fSPF = m_fSecondPerFrame = fSPF;
    g_fGT  = m_fGameTimer += m_fSecondPerFrame;
    m_fTmpTimer += m_fSecondPerFrame;    
    if (m_fTmpTimer > 1.0)
    {
        m_iGameFrame = m_iTmpGameFrame;
        m_fTmpTimer -= 1.0;
        m_iTmpGameFrame = 0;        
    }
    m_StartClock= m_EndClock;

#ifdef _DEBUG
    m_szTime = L"Game Time: ";
    m_szTime += std::to_wstring((int)m_fGameTimer);
    m_szTime += L"    FPS: ";
    m_szTime += std::to_wstring(m_iGameFrame);
    m_szTime += L"\n";
#endif
}
void    Time::Render()
{
#ifdef _DEBUG
    //OutputDebugString(m_szTime.c_str());
#endif
}
void    Time::Release()
{

}