#pragma once
#include "TProtocol.h"
#define MAX_RECV_SIZE 10000
class THost;
class TStreamPacket
{
public:	
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	int					m_iStartPos;
	int					m_iWritePos;
	int					m_iReadPos;
	THost*				m_pHost=nullptr;
public:
	void    Put(char* redvBuffer, int iSize);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

