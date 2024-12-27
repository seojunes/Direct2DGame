#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <string>
#include <list>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class TDxWrite
{
public:
	ID2D1Factory*		m_pd2dFactory = nullptr;
	ID2D1RenderTarget*  m_pd2dRT = nullptr;
	IDWriteFactory*		m_pDxWrite = nullptr;
	IDWriteTextFormat*  m_pTextFormat20 = nullptr;
	IDWriteTextFormat* m_pTextFormat50 = nullptr;
	ID2D1SolidColorBrush* m_pColorBrush = nullptr;
	std::list<std::wstring>	m_msgList;
	int		Add(std::wstring msg);
public:
	HRESULT   Create(IDXGISurface* pBackBuffer);
public:
	virtual void   Init();
	virtual void   Frame();
	virtual void   PreRender();
	virtual void   Render();
	virtual void   DirectDraw(D2D1_RECT_F layoutRect,
					  std::wstring msg, D2D1::ColorF Color = D2D1::ColorF(1, 0, 0, 1));
	void   Draw(D2D1_RECT_F layoutRect, std::wstring msg,
				D2D1::ColorF Color= D2D1::ColorF(1, 1, 1, 1));
	virtual void   PostRender();
	virtual void   Release();
};

