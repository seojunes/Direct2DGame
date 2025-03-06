#include "TThread.h"
unsigned int WINAPI TThread::HandleRunner(LPVOID arg)
{
	TThread* pThread = (TThread*)arg;
	// �ڵ鸵
	if(pThread)	pThread->RunThread();
	return 0;
}
void		 TThread::CreateThread()
{
	if (m_bRun == true) return;
	//CreateThread -> old
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, 
		(LPVOID)this, 0, &m_ithreadID);
	m_bRun = true;
}
TThread::TThread() : m_bRun(false)
{	
}
TThread::~TThread()
{
	CloseHandle((HANDLE)m_hThread);
}