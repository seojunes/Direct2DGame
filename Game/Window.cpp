#include "Window.h"
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
bool   Window::SetWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX); // 신고서류의 페이지수
    wcex.style = CS_HREDRAW | CS_VREDRAW; // 지상건출물
    wcex.lpfnWndProc = WndProc;  // 전화번호,주소,메일
    wcex.hInstance = m_hInstance;  // 허가권
    wcex.lpszClassName = L"KGCA";// 건축명
    wcex.hCursor = LoadCursorFromFile(L"../../data/curser/mega1.ani");
    //wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    // 운영체제에게 이런 윈도우를 사용할거야. (운영체제에세 신고한다.등록)
    WORD hr = RegisterClassExW(&wcex);
    return true;
}
// 윈도우창 생성
bool   Window::SetWindow(
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
bool   Window::MessageProcess()
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


