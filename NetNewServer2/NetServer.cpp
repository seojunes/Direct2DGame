#include "TNetwork.h"
int main()
{
    TNetwork tNet;
    tNet.Init();
    tNet.CreateServer(10000);
    tNet.Run();    
    tNet.Release();    
}
