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
// ������Ŭ���� ���
bool   Window::SetWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX); // �Ű����� ��������
    wcex.style = CS_HREDRAW | CS_VREDRAW; // ������⹰
    wcex.lpfnWndProc = WndProc;  // ��ȭ��ȣ,�ּ�,����
    wcex.hInstance = m_hInstance;  // �㰡��
    wcex.lpszClassName = L"KGCA";// �����
    wcex.hCursor = LoadCursorFromFile(L"../../data/curser/mega1.ani");
    //wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    //wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    // �ü������ �̷� �����츦 ����Ұž�. (�ü������ �Ű��Ѵ�.���)
    WORD hr = RegisterClassExW(&wcex);
    return true;
}
// ������â ����
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
// �޼��� ���ν���, ó��
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
        TranslateMessage(&msg); // Ű�� �����Ѵ�.
        DispatchMessage(&msg);  // ���ν����� �����Ѵ�.
        return true;
    }
    return false;
}


