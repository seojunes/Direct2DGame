#include "THost.h"
#include "TNetwork.h"
// �����ڸ� ���ؼ� ��Ŷ�� ��簪�� 0 ���� �ʱ�ȭ
THost::THost()
{
	ZeroMemory(&m_tPacket, sizeof(m_tPacket));
}

// ������ ���� �� ó��
bool THost::RunTCP(TNetwork& tNet)
{
	//��� ����
	char* pRecvMsg = (char*)&m_tPacket;
	// recv�� ����ؼ� ��� ũ�⸸ŭ ���� , ���ŵ� �����ʹ� msg[]�� ����
	int iRecvByte = recv(sock, &pRecvMsg[m_iRecvBytes], PACKET_HEADER_SIZE - m_iRecvBytes, 0);  // �ʱ� ���Žÿ� ������ �б�.
						// ����, �����͸� ���� �޸�, pRecvMsg�� m_tPacket ����ü�� �����͸� ������ �������� ó���Ѱ�.
						// m_iRecvBytes�� ������� ���� ����Ʈ ��
	TResult ret = Check(iRecvByte);             //�����͸� ���� �� ���� ���°� ���������� Ȯ��.
	if (ret == TResult::TNet_FALSE)             // ���� ������ �߻��� ��� ������ �����ϰ� false�� ��ȯ.
	{
		this->m_bConnect = false;
		return false;
	}
	if (ret == TResult::TNet_TRUE)              // ������ ���ٸ�, m_iRecvBytes�� ������ ����Ʈ�� (iRecvByte)�� ���Ͽ�, ���ũ�⸸ŭ �� �޾Ҵ��� Ȯ��.
	{
		m_iRecvBytes += iRecvByte;
		if (m_iRecvBytes > PACKET_HEADER_SIZE)
		{
			return true;
		}
	}

	if (m_iRecvBytes == PACKET_HEADER_SIZE)         //��� ���� �� ���� ����.
	{
		while (m_tPacket.ph.len > m_iRecvBytes)//m_iRecvbyte= 4
		{
			//    int recv(
	  //    SOCKET s,       // ����� ����
	  //    char* buf,      // �����͸� ������ ����
	  //    int len,        // ���� ũ�� (�ִ� ������ ������ ũ��)
	  //    int flags       // ������ �����ϴ� �÷��� (�⺻������ 0)
	  //    );
	  //    ����ŷ ��忡���� recv�� ��� ��ȯ�Ǹ�, ���� �����Ͱ� ������ WSAEWOULDBLOCK �����߻�.
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
		/*if (m_tPacket.ph.type == PACKET_DRUP_USER)
		{
			m_bConnect = false;
		}*/
		if (m_tPacket.ph.type == PACKET_CHAT_NAME_CS_ACK)
		{
			USER_NAME* pData = (USER_NAME*)m_tPacket.msg; // �޽����� ����ִ� ���� ������ pData�� ����
			memcpy(m_csName, pData->name, sizeof(char) * 32); // ��Ŷ�� �޼����� ����ִ� ������ ������ m_csName�� ����
			m_tPacket.ph.type = PACKET_JOIN_USER;				// ������ �����ߴٴ� ��Ŷ���� ��ȯ�ϴ� ����
			tNet.m_RecvPool.emplace_back(m_tPacket);			// ������Ŷ ť�� ��Ŷ�� �߰�.
		}
		else
		{
			tNet.m_RecvPool.emplace_back(m_tPacket);                // m_RecvPool�� �����ؼ� ���߿� ó��.
		}

		{
			m_iRecvBytes = 0;
			ZeroMemory(&m_tPacket, sizeof(m_tPacket));				// ��Ŷ ó�� �� m_tPacket �ʱ�ȭ.
		}
	}
	return true;
}

bool THost::RunUDP(TNetwork& tNet)
{
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


TResult THost::Check(int iCode)										// ���� ���� �˻�.
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