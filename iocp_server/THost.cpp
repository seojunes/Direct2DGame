#include "THost.h"
#include "TNetwork.h"

THost::THost()
{
    m_iRecvBytes = 0;               //수신된 바이트수
    m_bConnect = false;             // 연결상태
    ZeroMemory(&m_tPacket, sizeof(m_tPacket));      //m_tPacket : 패킷 데이터 구조체
    m_StreamPacket.m_pHost = this;                  // 스트림 패킷 (패킷 처리 및 저장용 객체)
}
// OVERLAPPED2 구조체를 기반으로 수신 또는 송신이 완료된 후의 처리 작업을 담당.
// 수신된 데이터를 패킷에 넣어준다.
void THost::Dispatch(DWORD dwTransfer, OVERLAPPED2* ov)
{
    //주) 받은 데이터를 패킷으로 분리해서 처리한다.
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
// WSARecv를 이용해서 비동기적으로 데이터를 수신하는 함수
// 비동기 I/O 작업이 성공적으로 시작되었을때, ERROR_IO_PENDING 오류는 정상적인 상태로 처리
// 만약 오류가 발생하면 연결을 끊고, false를 반환
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
// recv함수를 사용하여 TCP연결에서 데이터를 수신하는 함수
// 패킷을 받아오는 방식은 헤더를 먼저 받고, 헤더에 정의된 데이터 길이만큼 추가 데이터를 수신
// 데이터가 완전히 수신되면, 패킷을 네트워크객체(tNet)의 RecvPool에 넣고, 수신된 데이터 크기를 초기화
// PACKET_CHAT_NAME_CS_ACK 유형의 패킷을 처리할때, 사용자 이름을 복사하고 PACKET_JOIN_USER패킷을 전송준비
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
//소켓 통신에서 오류 처리를 담당하는 함수.
//SOCKET_ERROR가 발생하면, WSAEWOLDBLCK 에러발생시 비동기 작업을 기다려야하는 상태로 간주하고 그렇지 않으면 연결실패로 처리.
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