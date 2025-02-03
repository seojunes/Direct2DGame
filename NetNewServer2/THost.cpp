#include "THost.h"
#include "TNetwork.h"
THost::THost()
{
    ZeroMemory(&m_tPacket, sizeof(m_tPacket));
}
bool THost::Run(TNetwork& tNet)
{
    char* pRecvMsg = (char*)&m_tPacket;
    int iRecvByte = recv(sock, &pRecvMsg[m_iRecvBytes],
        PACKET_HEADER_SIZE - m_iRecvBytes, 0);
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