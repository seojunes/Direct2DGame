#include "THost.h"
#include "TNetwork.h"

THost::THost()
{
    m_iRecvBytes = 0;
    m_bConnect = false;    
    ZeroMemory(&m_tPacket, sizeof(m_tPacket));

	ZeroMemory(m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
	m_iStartPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
	m_pPacket = NULL;
}
void THost::Dispatch(DWORD dwTransfer, OVERLAPPED2* ov)
{
    //��) ���� �����͸� ��Ŷ���� �и��ؼ� ó���Ѵ�.
    if (ov->m_iFlag == OVERLAPPED2::MODE_RECV)
    {       
        Put((char*)&m_tPacket, dwTransfer);
		AsyncRecvTCP();
    }
    if (ov->m_iFlag == OVERLAPPED2::MODE_SEND)
    {        
    }
	
    delete ov;
}
bool THost::AsyncRecvTCP()
{
    OVERLAPPED2* ov = new OVERLAPPED2(OVERLAPPED2::MODE_RECV);

    char* pRecvMsg = (char*)&m_tPacket;
    m_wsaRecvBuffers.buf = pRecvMsg;
    m_wsaRecvBuffers.len = PACKET_MAX_PACKET_SIZE;
    DWORD dwRecvByte;
    DWORD dwFlags=0;
    int iRecvByte = WSARecv(sock,
        &m_wsaRecvBuffers,
        1,
        &dwRecvByte,
        &dwFlags,
        &ov->m_ov,
        nullptr);
   
    if (iRecvByte == SOCKET_ERROR)
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {        
            this->m_bConnect = false;
            return false;
        }
    }
    return true;
}
void THost::Put(char* recvBuffer,	int iRecvSize)
{
	// �޴� ������ �뷮�� �����ϸ�
	if (m_iWritePos + iRecvSize >= MAX_RECV_SIZE)
	{
		char strTemp[MAX_RECV_SIZE] = { 0, };
		memcpy(strTemp, &m_RecvBuffer[m_iStartPos], m_iReadPos);
		ZeroMemory(&m_RecvBuffer, sizeof(char) * MAX_RECV_SIZE);
		memcpy(&m_RecvBuffer, &strTemp, m_iReadPos);
		m_pPacket = (P_UPACKET)m_RecvBuffer;
		m_iStartPos = 0;
		m_iWritePos = m_iReadPos;
	}

	memcpy(&m_RecvBuffer[m_iWritePos], recvBuffer, iRecvSize);
	m_iWritePos += iRecvSize;
	m_iReadPos += iRecvSize;

	if (m_iReadPos < PACKET_HEADER_SIZE) return;
	// ��Ŷ�� ����
	m_pPacket = (UPACKET*)&m_RecvBuffer[m_iStartPos];
	// 1���� ��Ŷ �����ŭ �޾Ҵٸ�
	if (m_iReadPos >= m_pPacket->ph.len)
	{
		do {
			UPACKET    add;
			ZeroMemory(&add, sizeof(add));
			memcpy(&add, &m_RecvBuffer[m_iStartPos], m_pPacket->ph.len);			
			PacketProcess(add);
			// 1���� ��Ŷ�� ó���ϰ� ���� �ܿ��� ũ��
			m_iReadPos -= m_pPacket->ph.len;
			// ��Ŷ�� ���� ��ġ
			m_iStartPos += m_pPacket->ph.len;
			// �ܿ����� ���� ���
			if (add.ph.len == iRecvSize) break;
			// �ܿ����� ��Ŷ ����� ���� ���
			if (m_iReadPos < PACKET_HEADER_SIZE) break;

			m_pPacket = (P_UPACKET)m_RecvBuffer[m_iStartPos];
		} while (m_iReadPos >= m_pPacket->ph.len);
	}
};
void THost::PacketProcess(UPACKET& packet)
{
	if (packet.ph.type == PACKET_CHAT_NAME_CS_ACK)
	{
		USER_NAME* pData = (USER_NAME*)packet.msg;
		memcpy(m_csName, pData->name, sizeof(char) * 32);
		packet.ph.type = PACKET_JOIN_USER;
		m_tNet->AddRecvPacket(packet);
	}
	else
	{
		m_tNet->AddRecvPacket(packet);
	}
}