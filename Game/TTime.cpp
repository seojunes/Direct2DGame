#include "TTime.h"
#include <windows.h>
float   g_fSPF =0.0f;
float   g_fGT = 0.0f;
void    TTime::Init()
{   
    m_StartClock = system_clock::now();
    m_fTmpTimer = 0.0f;
}
void    TTime::Frame()
{
    m_iTmpGameFrame++;
	m_EndClock = system_clock::now();
    duration<float> sec = m_EndClock - m_StartClock;
    //microseconds sec = duration_cast<microseconds>(durationClock);
    g_fSPF = m_fSecondPerFrame = sec.count();
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
    m_szTime = std::to_wstring(m_fGameTimer);
    m_szTime += L" ";
    m_szTime += std::to_wstring(m_fSecondPerFrame);
    m_szTime += L" ";
    m_szTime += std::to_wstring(m_iGameFrame);
    m_szTime += L"\n";
#endif
}
void    TTime::Render()
{
#ifdef _DEBUG
    //OutputDebugString(m_szTime.c_str());
#endif
}
void    TTime::Release()
{

}