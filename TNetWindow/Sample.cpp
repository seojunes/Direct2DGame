#include "Sample.h"
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 200:
        {
            char buffer[MAX_PATH] = { 0, };
            SendMessageA(m_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
            m_Net.SendPacket(m_Net.m_Sock, buffer, PACKET_CHAT_MSG);
        }break;
        }
    }break;
    }
    return 0;
}
void    Sample::Init()
{
    m_hEdit = CreateWindow(L"edit", NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        500, 10, 200, 25, m_hWnd, (HMENU)100,
        m_hInstance, NULL);
    m_hSendBtn = CreateWindow(L"button", L"Send",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        700, 10, 50, 25, m_hWnd, (HMENU)200,
        m_hInstance, NULL);
    m_hListBtn = CreateWindow(L"listbox", NULL,
        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
        0, 0, 400, 600, m_hWnd, (HMENU)300,
        m_hInstance, NULL);

    m_Net.Init();
    m_Net.Connect("192.168.0.44", 10000);
}
void    Sample::Frame()
{
    m_Net.Frame();
    if (m_Net.m_DataMsg.size() > 0)
    {
        for (int iMsg = 0; iMsg < m_Net.m_DataMsg.size(); iMsg++)
        {
            SendMessageA(m_hListBtn,
                LB_ADDSTRING, 0,
                (LPARAM)m_Net.m_DataMsg[iMsg].c_str());
        }
        m_Net.m_DataMsg.clear();
        int iMsgCounter = SendMessage(m_hListBtn,
            LB_GETCOUNT, 0, 0);
        if (iMsgCounter > 30)
        {
            SendMessage(m_hListBtn, LB_RESETCONTENT, 0, 0);
        }
    }
}
void    Sample::Render()
{

}
void    Sample::Release()
{
    m_Net.Release();
}

GAME_RUN