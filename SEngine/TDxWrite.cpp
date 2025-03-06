#include "TDxWrite.h"
#include "TStd.h"

int		TDxWrite::Add(std::wstring msg)
{
	TSync lock(this);
	UINT iNumsize = m_msgList.size();
	if (iNumsize > 20)
	{
		m_msgList.pop_front();
	}
	m_msgList.emplace_back(msg);
	return iNumsize;
}
HRESULT   TDxWrite::Create(IDXGISurface* pBackBuffer)
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pd2dFactory);
	if (FAILED(hr))
	{
		DX_CHECK(hr, _T(__FUNCTION__));
		return hr;
	}
	
	UINT dpi = GetDpiForWindow(g_hWnd);

	// UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // dwrite¿¬µ¿ ÇÊ¼ö
	D2D1_RENDER_TARGET_PROPERTIES rtp;
	ZeroMemory(&rtp, sizeof(rtp));
	rtp.type= D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, 
										D2D1_ALPHA_MODE_PREMULTIPLIED);
	rtp.dpiX = dpi;
	rtp.dpiY = dpi;

	hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
		pBackBuffer,
		&rtp, &m_pd2dRT);

	if (FAILED(hr))
	{
		DX_CHECK(hr, _T(__FUNCTION__));
		return hr;
	}
	m_pd2dRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow), &m_pColorBrush);

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&m_pDxWrite);
	if (SUCCEEDED(hr))
	{
		hr = m_pDxWrite->CreateTextFormat(
			L"±Ã¼­", NULL,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20, L"ko-kr", // "en-us"
			&m_pTextFormat20);

		if (FAILED(hr))
		{
			DX_CHECK(hr, _T(__FUNCTION__));
			return hr;
		}
		hr = m_pDxWrite->CreateTextFormat(
			L"±Ã¼­", NULL,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			50, L"ko-kr", // "en-us"
			&m_pTextFormat50);

		if (FAILED(hr))
		{
			DX_CHECK(hr, _T(__FUNCTION__));
			return hr;
		}
	}

	
	return hr;
}
void   TDxWrite::Init()
{	

}
void   TDxWrite::Frame()
{

}
void   TDxWrite::Render()
{	
}
void   TDxWrite::DirectDraw(D2D1_RECT_F layoutRect, 
			std::wstring msg, D2D1::ColorF Color)
{
	m_pd2dRT->BeginDraw();
	if (m_pd2dRT)
	{
		//D2D1_COLOR_F Color = D2D1::ColorF(1,1,0,1);
		m_pColorBrush->SetColor(Color);
		m_pd2dRT->DrawText(msg.c_str(), msg.size(), m_pTextFormat20, &layoutRect, m_pColorBrush);
	}
	m_pd2dRT->EndDraw();
}
void   TDxWrite::Draw(D2D1_RECT_F layoutRect, std::wstring msg, D2D1::ColorF Color)
{
	if (m_pd2dRT)
	{
		//D2D1_COLOR_F Color = D2D1::ColorF(1,1,0,1);
		m_pColorBrush->SetColor(Color);
		m_pd2dRT->DrawText(msg.c_str(),	msg.size(), m_pTextFormat20, &layoutRect,	m_pColorBrush);
	}
}
void   TDxWrite::PreRender()
{
	if (m_pd2dRT)m_pd2dRT->BeginDraw();
}
void   TDxWrite::PostRender()
{	
	D2D1_RECT_F rt = { 300.0f, 0.0f, 800.0f, 600.0f };
	{
		TSync lock(this);
		for (auto data : m_msgList)
		{
			rt.top += 20;
			Draw(rt, data);
		}
	}
	if (m_pd2dRT) m_pd2dRT->EndDraw();
}
void   TDxWrite::Release()
{
	m_msgList.clear();

	if (m_pColorBrush)m_pColorBrush->Release();
	if(m_pd2dFactory)m_pd2dFactory->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pDxWrite)m_pDxWrite->Release();
	if (m_pTextFormat20)m_pTextFormat20->Release();
	if (m_pTextFormat50)m_pTextFormat50->Release();
}
