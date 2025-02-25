#include "TNetworkUDP.h"
#include "TNetworkTCP.h"
int main()
{
    //TNetwork tNet;
    TNetworkTCP tNet(std::make_shared<TEventSelect>());
    tNet.Init();
    tNet.CreateServer(10000);
    tNet.Run();    
    tNet.Release();    
}
