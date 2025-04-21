#include "TWindow.h"
POINT g_ptClientSize;
HWND  g_hWnd;
short g_nMouseWheelDelta;
TWindow* g_pWindow = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return  DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT TWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEWHEEL:
    {
        // +120, -120 ���� ��ġ
        m_nMouseWheelDelta = (short)HIWORD(wParam);
        //
        //m_nMouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);     
        g_nMouseWheelDelta = m_nMouseWheelDelta;
    };
    }
    return 0;
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
    //PM_REMOVE�� ���ؼ� �޽����� �����Ȱ� ���ÿ� �����Ѵ�.
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
//�����ڸ� ���ؼ� g_pWindow����.
TWindow::TWindow()
{
    g_pWindow = this;
}

