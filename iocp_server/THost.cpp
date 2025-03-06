#include "THost.h"
#include "TNetwork.h"

THost::THost()
{
    m_iRecvBytes = 0;               //���ŵ� ����Ʈ��
    m_bConnect = false;             // �������
    ZeroMemory(&m_tPacket, sizeof(m_tPacket));      //m_tPacket : ��Ŷ ������ ����ü
    m_StreamPacket.m_pHost = this;                  // ��Ʈ�� ��Ŷ (��Ŷ ó�� �� ����� ��ü)
}
// OVERLAPPED2 ����ü�� ������� ���� �Ǵ� �۽��� �Ϸ�� ���� ó�� �۾��� ���.
// ���ŵ� �����͸� ��Ŷ�� �־��ش�.
void THost::Dispatch(DWORD dwTransfer, OVERLAPPED2* ov)
{
    //��) ���� �����͸� ��Ŷ���� �и��ؼ� ó���Ѵ�.
    if (ov->m_iFlag == OVERLAPPED2::MODE_RECV)
    {       
        m_StreamPacket.Put((char*)&m_tPacket, dwTransfer);
        //m_tNet->m_RecvPool.emplace_back(m_tPacket);
    }
    if (ov->m_iFlag == OVERLAPPED2::MODE_SEND)
    {        
    }
    delete ov;
}
// WSARecv�� �̿��ؼ� �񵿱������� �����͸� �����ϴ� �Լ�
// �񵿱� I/O �۾��� ���������� ���۵Ǿ�����, ERROR_IO_PENDING ������ �������� ���·� ó��
// ���� ������ �߻��ϸ� ������ ����, false�� ��ȯ
bool THost::AsyncRecvTCP(TNetwork& tNet)
{
    m_tNet = &tNet;
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
// recv�Լ��� ����Ͽ� TCP���ῡ�� �����͸� �����ϴ� �Լ�
// ��Ŷ�� �޾ƿ��� ����� ����� ���� �ް�, ����� ���ǵ� ������ ���̸�ŭ �߰� �����͸� ����
// �����Ͱ� ������ ���ŵǸ�, ��Ŷ�� ��Ʈ��ũ��ü(tNet)�� RecvPool�� �ְ�, ���ŵ� ������ ũ�⸦ �ʱ�ȭ
// PACKET_CHAT_NAME_CS_ACK ������ ��Ŷ�� ó���Ҷ�, ����� �̸��� �����ϰ� PACKET_JOIN_USER��Ŷ�� �����غ�
bool THost::RunTCP(TNetwork& tNet)
{
    m_tNet = &tNet;
    char* pRecvMsg = (char*)&m_tPacket;
    int iRecvByte = recv(sock, &pRecvMsg[m_iRecvBytes],PACKET_HEADER_SIZE - m_iRecvBytes, 0);
    TResult ret = Check(iRecvByte);
    if (ret == TResult::TNet_FALSE)
    {
        this->m_bConnect = false;
        return false;
    }
    if (ret == TResult::TNet_TRUE)
    {
        m_iRecvBytes += iRecvByte;
        if (m_iRecvBytes > PACKET_HEADER_SIZE)
        {
            return true;
        }
    }

    if (m_iRecvBytes == PACKET_HEADER_SIZE)
    {
        while (m_tPacket.ph.len > m_iRecvBytes)//m_iRecvbyte= 4
        {
            int iRecvByte = recv(sock,
                &pRecvMsg[m_iRecvBytes],
                m_tPacket.ph.len - m_iRecvBytes, 0);
            TResult rRet = Check(iRecvByte);
            if (rRet == TResult::TNet_TRUE)
            {
                m_iRecvBytes += iRecvByte;
            }
            if (rRet == TResult::TNet_FALSE)
            {
                m_bConnect = false;
                return false;
            }
            
        }
      /*  if (m_tPacket.ph.type == PACKET_DRUP_USER)
        {
            m_bConnect = false;
        }*/

        if (m_tPacket.ph.type == PACKET_CHAT_NAME_CS_ACK)
        {
            USER_NAME* pData = (USER_NAME*)m_tPacket.msg;
            memcpy(m_csName, pData->name, sizeof(char)*32);
            m_tPacket.ph.type = PACKET_JOIN_USER;
            tNet.m_RecvPool.emplace_back(m_tPacket);
        }
        else
        {
            tNet.m_RecvPool.emplace_back(m_tPacket);
        }
        m_iRecvBytes = 0;
        ZeroMemory(&m_tPacket, sizeof(m_tPacket));
    }
    return true;
}
bool THost::RunUDP(TNetwork& tNet)
{
    m_tNet = &tNet;
    //char* pRecvMsg = (char*)&m_tPacket;
    //int iRecvByte = recvfrom(sock, &pRecvMsg[m_iRecvBytes],
    //    PACKET_HEADER_SIZE - m_iRecvBytes, 0);
    //TResult ret = Check(iRecvByte);
    //if (ret == TResult::TNet_FALSE)
    //{
    //    this->m_bConnect = false;
    //    return false;
    //}
    //if (ret == TResult::TNet_TRUE)
    //{
    //    m_iRecvBytes += iRecvByte;
    //    if (m_iRecvBytes > PACKET_HEADER_SIZE)
    //    {
    //        return true;
    //    }
    //}

    //if (m_iRecvBytes == PACKET_HEADER_SIZE)
    //{
    //    while (m_tPacket.ph.len > m_iRecvBytes)//m_iRecvbyte= 4
    //    {
    //        int iRecvByte = recv(sock,
    //            &pRecvMsg[m_iRecvBytes],
    //            m_tPacket.ph.len - m_iRecvBytes, 0);
    //        TResult rRet = Check(iRecvByte);
    //        if (rRet == TResult::TNet_TRUE)
    //        {
    //            m_iRecvBytes += iRecvByte;
    //        }
    //        if (rRet == TResult::TNet_FALSE)
    //        {
    //            m_bConnect = false;
    //            return false;
    //        }

    //    }
    //    if (m_tPacket.ph.type == PACKET_CHAT_NAME_CS_ACK)
    //    {
    //        USER_NAME* pData = (USER_NAME*)m_tPacket.msg;
    //        memcpy(m_csName, pData->name, sizeof(char) * 32);
    //        m_tPacket.ph.type = PACKET_JOIN_USER;
    //        tNet.m_RecvPool.emplace_back(m_tPacket);
    //    }
    //    else
    //    {
    //        tNet.m_RecvPool.emplace_back(m_tPacket);
    //    }
    //    m_iRecvBytes = 0;
    //    ZeroMemory(&m_tPacket, sizeof(m_tPacket));
    //}
    //return true;
    return true;
}
//���� ��ſ��� ���� ó���� ����ϴ� �Լ�.
//SOCKET_ERROR�� �߻��ϸ�, WSAEWOLDBLCK �����߻��� �񵿱� �۾��� ��ٷ����ϴ� ���·� �����ϰ� �׷��� ������ ������з� ó��.
TResult THost::Check(int iCode)
{
    if (iCode == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            return TResult::TNet_FALSE;
        }
        return TResult::TNet_EWOULDBLOCK;
    }
    return TResult::TNet_TRUE;
}