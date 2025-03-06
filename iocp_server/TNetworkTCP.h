#pragma once
#include "TNetwork.h"
class TNetworkTCP : public TNetwork
{
public:
	SOCKET    Accept() override;
	bool	CheckAccept(int iCode)override;
public:
	virtual bool  CreateServer(int iPort) override;
	virtual bool  Run() override;
	virtual bool  RecvRun() override;
	virtual int   SendPacket(SOCKET sock,const char* msg,	WORD type) override;
	virtual int   SendPacket(SOCKADDR_IN addr, const char* msg, WORD type) override;
	int     SendPacket(THost* host, const char* msg, WORD type);
	virtual bool  PostProcess() override;
	virtual int   Send(THost&, UPACKET&)override;	
public:
	TNetworkTCP(std::shared_ptr<TNetModel> pModel)
		: TNetwork(pModel)
	{
	}
};

