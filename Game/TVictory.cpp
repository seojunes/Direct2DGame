#include "TVictory.h"
void TVictory::Frame()
{
	if (m_vPos.y <= 300.0f)
	{
		m_vPos.y += m_vDir.y * m_fSpeed * g_fSPF;
		SetPosition(m_vPos);
	}
}