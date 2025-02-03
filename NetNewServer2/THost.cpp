#include "THost.h"
#include "TNetwork.h"
// 생성자를 통해서 패킷의 모든값을 0 으로 초기화
THost::THost()
{
	ZeroMemory(&m_tPacket, sizeof(m_tPacket));
}

// 데이터 수신 및 처리
bool THost::Run(TNetwork& tNet)
{
	//헤더 수신
	char* pRecvMsg = (char*)&m_tPacket;
	// recv를 사용해서 헤더 크기만큼 수신 , 수신된 데이터는 msg[]에 저장
	int iRecvByte = recv(sock, &pRecvMsg[m_iRecvBytes], PACKET_HEADER_SIZE - m_iRecvBytes, 0);  // 초기 수신시에 데이터 읽기.
						// 소켓, 데이터를 받을 메모리, pRecvMsg는 m_tPacket 구조체의 데이터를 포인터 형식으로 처리한것.
						// m_iRecvBytes는 현재까지 받은 바이트 수
	TResult ret = Check(iRecvByte);             //데이터를 수신 후 수신 상태가 정상적인지 확인.
	if (ret == TResult::TNet_FALSE)             // 수신 오류가 발생한 경우 연결을 종료하고 false를 반환.
	{
		this->m_bConnect = false;
		return false;
	}
	if (ret == TResult::TNet_TRUE)              // 오류가 없다면, m_iRecvBytes에 수신한 바이트수 (iRecvByte)를 더하여, 헤더크기만큼 다 받았는지 확인.
	{
		m_iRecvBytes += iRecvByte;
		if (m_iRecvBytes > PACKET_HEADER_SIZE)
		{
			return true;
		}
	}

	if (m_iRecvBytes == PACKET_HEADER_SIZE)         //헤더 수신 후 본문 수신.
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
			memcpy(m_csName, pData->name, sizeof(char) * 32);
			m_tPacket.ph.type = PACKET_JOIN_USER;
			tNet.m_RecvPool.emplace_back(m_tPacket);
		}
		else
		{
			tNet.m_RecvPool.emplace_back(m_tPacket);                // m_RecvPool에 저장해서 나중에 처리.
		}

		{
			m_iRecvBytes = 0;
			ZeroMemory(&m_tPacket, sizeof(m_tPacket));				// 패킷 처리 후 초기화.
		}
	}
	return true;
}

TResult THost::Check(int iCode)										// 수신 상태 검사.
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