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
// ������Ŭ���� ���
bool   TWindow::SetWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX); // �Ű����� ��������
    wcex.style = CS_HREDRAW | CS_VREDRAW; // ������⹰
    wcex.lpfnWndProc = WndProc;  // ��ȭ��ȣ,�ּ�,����
    wcex.hInstance = m_hInstance;  // �㰡��
    wcex.lpszClassName = L"KGCA";// �����
    //wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    // �ü������ �̷� �����츦 ����Ұž�. (�ü������ �Ű��Ѵ�.���)
    WORD hr = RegisterClassExW(&wcex);
	return true;
}
// ������â ����
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
// �޼��� ���ν���, ó��
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
        TranslateMessage(&msg); // Ű�� �����Ѵ�.
        DispatchMessage(&msg);  // ���ν����� �����Ѵ�.
        return true;
    }
    return false;
}

//// �޼��� ���ν���, ó��
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
//            TranslateMessage(&msg); // Ű�� �����Ѵ�.
//            DispatchMessage(&msg);  // ���ν����� �����Ѵ�.
//        }
//        else
//        {
//            
//            m_EndClock = std::clock();
//            std::clock_t t1 = m_EndClock - tick; // 1000����
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
//                // ���ӷ���
//                GameRun();       
//              /*  tick = m_EndClock;
//            }*/
//            m_StartClock = m_EndClock;
//        }
//    }
//	return true;
//}

