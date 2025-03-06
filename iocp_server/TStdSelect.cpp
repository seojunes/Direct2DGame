#include "TNetModel.h"
#include "TNetwork.h"
void  TStdSelect::Rebuild()
{
	FD_ZERO(&m_rSet);
	FD_ZERO(&m_wSet);
	FD_ZERO(&m_eSet);

	FD_SET(m_Sock, &m_rSet);
	FD_SET(m_Sock, &m_wSet);
	FD_SET(m_Sock, &m_eSet);
	for (auto& host : m_pNet->m_HostList)
	{
		if (host.m_bConnect == true)
		{
			FD_SET(host.sock, &m_rSet);
			FD_SET(host.sock, &m_wSet);
			FD_SET(host.sock, &m_eSet);
		}
	}
}
bool  TStdSelect::Init(TNetwork* pNet) {

	m_pNet = pNet;
	m_Sock = m_pNet->m_Sock;	
	return true;
}
bool  TStdSelect::NetEvent_Check(int iEvent)
{
	if (iEvent == SOCKET_ERROR)
	{		
		exit(1);
		return false;
	}	
	// timeout
	if (iEvent == 0) return false;
	return true;
}
bool  TStdSelect::Run() {

	Rebuild();

	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 1;
	int iIndex = select(0, &m_rSet, &m_wSet, &m_eSet, &timeout);
	if (!NetEvent_Check(iIndex))
	{
		return false;
	}
	if (FD_ISSET(m_Sock, &m_eSet))
	{
		exit(1);
		return false;
	}
	//listen sock
	if (FD_ISSET(m_Sock, &m_rSet))
	{
		// accept
		SOCKET sock = m_pNet->Accept();		
	}
	for (auto& host : m_pNet->m_HostList)
	{
		if (host.m_bConnect == false)
		{
			continue;
		}
		if (FD_ISSET(host.sock, &m_rSet))
		{
			host.RunTCP(*m_pNet);
		}
		if (FD_ISSET(host.sock, &m_wSet))
		{			
		}
		if (FD_ISSET(host.sock, &m_eSet))
		{
			host.m_bConnect = false;
		}
	}
	return true;
}