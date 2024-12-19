#include "TWindow.h"
POINT g_ptClientSize;
HWND  g_hWnd;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return  DefWindowProc(hWnd, message, wParam, lParam);
}
// 윈도우클래스 등록
bool   TWindow::SetWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX); // 신고서류의 페이지수
    wcex.style = CS_HREDRAW | CS_VREDRAW; // 지상건출물
    wcex.lpfnWndProc = WndProc;  // 전화번호,주소,메일
    wcex.hInstance = m_hInstance;  // 허가권
    wcex.lpszClassName = L"KGCA";// 건축명
    //wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    // 운영체제에게 이런 윈도우를 사용할거야. (운영체제에세 신고한다.등록)
    WORD hr = RegisterClassExW(&wcex);
	return true;
}
// 윈도우창 생성
bool   TWindow::SetWindow(
    std::wstring title,
    UINT iWindowX, UINT iWindowY)
{
    this->m_ptClientSize.x = iWindowX;
    this->m_ptClientSize.y = iWindowY;
    g_ptClientSize = m_ptClientSize;

    RECT rt = { 0,0,iWindowX, iWindowY };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(
        L"KGCA", title.c_str(),
        WS_OVERLAPPEDWINDOW,//WS_OVERLAPPED,
        100, 100, rt.right - rt.left, rt.bottom - rt.top,
        nullptr, nullptr,
        m_hInstance, nullptr);
    if (!hWnd)
    {
        return false;
    }
   
    GetWindowRect(hWnd, &m_rtWindow);
    GetClientRect(hWnd, &m_rtClient);
    ShowWindow(hWnd, SW_SHOW);
    m_hWnd = hWnd;
    g_hWnd = hWnd;
	return true;
}
// 메세지 프로시쳐, 처리
bool   TWindow::MessageProcess()
{
    MSG msg;   
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_bRun = false;
            return false;
        };
        TranslateMessage(&msg); // 키를 번역한다.
        DispatchMessage(&msg);  // 프로시져로 전송한다.
        return true;
    }
    return false;
}

//// 메세지 프로시쳐, 처리
//bool   TWindow::MessageProcess() 
//{
//    MSG msg;
//    std::clock_t tick = std::clock();
//    std::clock_t m_StartClock = std::clock();
//    std::clock_t m_EndClock = std::clock();
//    double m_fGameTimer = 0.0;
//    double m_fTmpTimer = 0.0f;
//    double m_fSecondPerFrame = 0.0f;
//    UINT m_iGameFrame = 0;
//    while (m_bRun)
//    {
//        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg); // 키를 번역한다.
//            DispatchMessage(&msg);  // 프로시져로 전송한다.
//        }
//        else
//        {
//            
//            m_EndClock = std::clock();
//            std::clock_t t1 = m_EndClock - tick; // 1000단위
//            
//            m_fSecondPerFrame = (m_EndClock - m_StartClock) / (double)CLOCKS_PER_SEC;
//            m_fGameTimer += m_fSecondPerFrame;
//            m_fTmpTimer += m_fSecondPerFrame;
//            if (m_fTmpTimer > 1.0)
//            {
//                std::wstring m_szTime = std::to_wstring(m_fGameTimer);
//                m_szTime += L" ";
//                m_szTime += std::to_wstring(m_fSecondPerFrame);
//                m_szTime += L" ";
//                m_szTime += std::to_wstring(m_iGameFrame);
//                m_szTime += L"\n";
//                OutputDebugString(m_szTime.c_str());
//                m_fTmpTimer -= 1.0;
//                m_iGameFrame = 0;
//            }
//            
//            /*if (t1 >= 10)
//            {*/
//                m_iGameFrame++;
//                // 게임로직
//                GameRun();       
//              /*  tick = m_EndClock;
//            }*/
//            m_StartClock = m_EndClock;
//        }
//    }
//	return true;
//}

