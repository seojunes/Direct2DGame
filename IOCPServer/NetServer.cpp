#include "TNetwork.h"
int main()
{
    //OVERLAPPED2* ov1 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov2 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov3 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov4 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov5 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov6 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov7 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov8 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //OVERLAPPED2* ov9 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //delete ov1;
    //OVERLAPPED2* ov10 = new OVERLAPPED2(OVERLAPPED2::MODE_SEND);
    //delete ov2;
    //delete ov3;
    //delete ov4;
    //delete ov5;
    //delete ov6;
    //delete ov7;
    //delete ov8;
    //delete ov9;
    //delete ov10;

    TNetwork tNet;
    tNet.Init();
    tNet.CreateServer(10000);
    tNet.Run();    
    tNet.Release();    
}
