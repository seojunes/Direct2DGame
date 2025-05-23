#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <tchar.h> // _T("") == L""
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>
#include <atlconv.h>  // A2W
#include <wrl.h> // windows runtime c++ template library
#include "TVertex.h"
#include "TMath.h"
#pragma comment(lib,"d3d11.lib") // 加己 眠啊辆加己 
#pragma comment(lib,"d3dcompiler.lib") // 加己 眠啊辆加己 

#ifndef _DEBUG
#pragma comment(lib, "DirectXTK_R.lib")
#pragma comment(lib,"TEngine_R.lib")
#else
#pragma comment(lib, "DirectXTK_d.lib")
#pragma comment(lib,"TEngine_d.lib")
#endif



#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

using namespace std;
using namespace Microsoft::WRL;

typedef basic_string<TCHAR> T_STR;
typedef basic_string<wchar_t> W_STR;
typedef basic_string<char>  C_STR;
typedef vector<T_STR>		T_STR_VECTOR;
typedef basic_string<TCHAR>::iterator	T_ITOR;
typedef basic_string<wchar_t>::iterator	W_ITOR;
typedef basic_string<char>::iterator	C_ITOR;
typedef vector<T_STR>		T_ARRAY_ITOR;
typedef vector<DWORD>				DWORD_VECTOR;
typedef	vector< DWORD >::iterator	DWORD_VECTOR_ITOR;
typedef list<DWORD>					DWORD_LIST;
typedef list<DWORD>::iterator		DWORD_LIST_ITOR;
typedef list< HANDLE >				HANDLE_LIST;
typedef	list< HANDLE >::iterator	HANDLE_LIST_ITOR;
static std::wstring to_mw(const std::string& _src)
{
    USES_CONVERSION;
    return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
    USES_CONVERSION;
    return std::string(W2A(_src.c_str()));
};
struct TGameKey
{
    //DWORD frontMove;// w or VK_UP
    //DWORD Attack;// Lbutton or VK_ENTER
    DWORD dwWkey;
    DWORD dwSkey;
    DWORD dwAkey;
    DWORD dwDkey;
    DWORD dwLeftClick;
    DWORD dwRightClick;
    DWORD dwMiddleClick;
    DWORD dwExit;
    DWORD dwSpace;
};

enum { KEY_FREE = 0, KEY_PUSH, KEY_HOLD, KEY_UP };

extern POINT g_ptClientSize;
extern POINT g_ptMouse;
extern HWND  g_hWnd;
extern TGameKey g_GameKey;
extern float    g_fSPF;
extern float    g_fGT;
extern POINT    g_ptDeltaMouse;
extern short    g_nMouseWheelDelta;
static void DX_CHECK(HRESULT hr, const TCHAR* function)
{
    if (FAILED(hr))
    {
        LPWSTR output;
        WCHAR buffer[256] = { 0, };
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS |
            FORMAT_MESSAGE_ALLOCATE_BUFFER,
            NULL, hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&output, 0, NULL);
        wsprintf(buffer, L"File=%ls\nLine=%d\nFuction=%ls", _T(__FILE__), __LINE__, function);
        std::wstring msg = buffer;
        msg += L"\n";
        msg += output;

        std::wstring title = L"ERROR(";
        title += std::to_wstring(hr);
        title += L")";
        MessageBox(NULL, msg.c_str(),
            title.c_str(), MB_OK);
    }
}