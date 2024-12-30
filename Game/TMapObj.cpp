#include "TMapObj.h"
#include "TDevice.h"
#include <time.h>
TMapObj::TMapObj()
{
}
TMapObj::TMapObj(UINT iCellX, UINT iCellY)
{
	SetCellCounter(iCellX, iCellY);
	m_rtScreen.SetS(0.0f, 0.0f, (float)g_ptClientSize.x, (float)g_ptClientSize.y);
	m_vCellDistance.x = (float)g_ptClientSize.x / (float)m_iNumCellCol;
	m_vCellDistance.y = (float)g_ptClientSize.y / (float)m_iNumCellRow;
}
TMapObj::TMapObj(TRect rt, UINT iCellX, UINT iCellY)
{
	m_rtScreen = rt;
	m_vCellDistance.x = (rt.v2.x - rt.v1.x) / iCellX;
	m_vCellDistance.y = (rt.v2.y - rt.v1.y) / iCellY;
	SetCellCounter(iCellX, iCellY);
}
bool	TMapObj::Create(TWorld* pWorld)
{
	m_pWorld = pWorld;
	SetVertexData();
	if (!CreateVertexBuffer())
	{
		return false;
	}
	SetIndexData();
	if (!CreateIndexBuffer())
	{
		return false;
	}
	/*if (!CreateVertexShader())
	{
		return false;
	}
	if (!CreatePixelShader())
	{
		return false;
	}
	if (!CreateInputLayout())
	{
		return false;
	}*/
	return true;
}
void TMapObj::Frame()
{
	//srand(time(NULL));
	//UINT iVertex = rand() % m_vVertexList.size();
	///*for (int i = 0; i < m_vVertexList.size(); i++)
	//{
	//	m_vVertexList[i].v.x += g_fSPF * cosf(g_fGT);
	//}*/

	//static float vPos = 0.0f;
	//static float tEnd = 1000.0f;
	//vPos += g_fSPF * 0.0f;
	//tEnd   += g_fSPF * 0.0f;
	//UINT xSize = m_pTexture->m_TexDesc.Width;
	//UINT ySize = m_pTexture->m_TexDesc.Height;
	//m_vVertexList[0].t = { 0.0f,vPos / ySize };
	//m_vVertexList[1].t = { 1.0f,vPos / ySize };
	//m_vVertexList[2].t = { 0.0f,tEnd / ySize };
	//m_vVertexList[3].t = { 1.0f,tEnd / ySize };
	///*m_vVertexList[0].t = { vPos / xSize, 0.0f };
	//m_vVertexList[1].t = { tEnd / xSize, 0.0f };
	//m_vVertexList[2].t = { vPos / xSize, 1.0f };
	//m_vVertexList[3].t = { tEnd / xSize, 1.0f }; */
	//TDevice::m_pd3dContext->UpdateSubresource(
	//	m_pVertexBuffer.Get(), 0, nullptr,
	//	&m_vVertexList.at(0), 0, 0);
}
void TMapObj::UpdateVertexData()
{

}
void TMapObj::SetCellCounter( UINT iCol, UINT iRow)
{
	m_iNumRow = iRow + 1;
	m_iNumCol = iCol + 1;
	m_iNumCellRow = iRow;
	m_iNumCellCol = iCol;

}
void TMapObj::SetVertexData()
{
	// -1000 ~ + 1000  :   0
	// 0    ~  2000    :   1000
	/*m_iNumCellRow = m_iNumRow - 1;
	m_iNumCellCol = m_iNumCol - 1;*/
	m_iNumVertex = m_iNumRow * m_iNumCol;
	m_iNumIndex = m_iNumCellRow * m_iNumCellCol * 2 * 3;
	//TObject::SetVertexData();	
	m_vVertexList.resize(m_iNumRow * m_iNumCol);
	m_vScreenList.resize(m_iNumRow * m_iNumCol);
	m_Cells.resize(m_iNumCellRow * m_iNumCellCol);
	// 0  1  2  
	// 3  4  5 
	// 6  7  8
	// iNumCell  = (3-1) * (3-1)
	float fOffsetU = 1.0f / (float)m_iNumCellCol;
	float fOffsetV = 1.0f / (float)m_iNumCellRow;
	for (UINT iRow = 0; iRow < m_iNumRow; iRow++)
	{
		for (UINT iCol = 0; iCol < m_iNumCol; iCol++)
		{
			float x = iCol * m_vCellDistance.x;
			float y = iRow * m_vCellDistance.y;
			m_vScreenList[iRow * m_iNumCol + iCol].x = m_rtScreen.v1.x + x;
			m_vScreenList[iRow * m_iNumCol + iCol].y = m_rtScreen.v1.y + y;
			m_vVertexList[iRow * m_iNumCol + iCol].v = ScreenToNDC(x, y, g_ptClientSize);
			m_vVertexList[iRow * m_iNumCol + iCol].c = { iCol / 2.0f, iRow / 2.0f, 1.0f,1.0f };
			m_vVertexList[iRow * m_iNumCol + iCol].t.x = iCol;
			m_vVertexList[iRow * m_iNumCol + iCol].t.y = iRow;
		}
	}
}
void TMapObj::SetIndexData()
{
	m_vIndexList.resize(m_iNumIndex);
	// 0  1  2  
	// 3  4  5 
	// 6  7  8
	// iNumCell  = (3-1) * (3-1)

	UINT iIndex = 0;
	for (UINT iRowCell = 0; iRowCell < m_iNumCellRow; iRowCell++)
	{
		for (UINT iColCell = 0; iColCell < m_iNumCellCol; iColCell++)
		{
			UINT iNextCol = iColCell + 1;
			UINT iNextRow = iRowCell + 1;
			m_vIndexList[iIndex + 0] = iRowCell * m_iNumCol + iColCell;
			m_vIndexList[iIndex + 1] = iRowCell * m_iNumCol + iNextCol;
			m_vIndexList[iIndex + 2] = iNextRow * m_iNumCol + iColCell;

			m_vIndexList[iIndex + 3] = m_vIndexList[iIndex + 2];
			m_vIndexList[iIndex + 4] = m_vIndexList[iIndex + 1];
			m_vIndexList[iIndex + 5] = iNextRow * m_iNumCol + iNextCol;

			UINT iCell = iRowCell * m_iNumCellCol + iColCell;
			float x1 = m_vScreenList[m_vIndexList[iIndex + 0]].x;
			float y1 = m_vScreenList[m_vIndexList[iIndex + 0]].y;
			float x2 = m_vScreenList[m_vIndexList[iIndex + 5]].x;
			float y2 = m_vScreenList[m_vIndexList[iIndex + 5]].y;
			m_Cells[iCell].rt.SetP(x1, y1, x2, y2);
			m_Cells[iCell].iTexID = 0;
			iIndex += 6;
		}
	}

	m_pTexs[0] = I_Tex.Load(L"../../data/texture/Map.png");
	m_pTexs[1] = I_Tex.Load(L"../../data/texture/kgcalogo.bmp");
	m_pTexs[2] = I_Tex.Load(L"../../data/texture/kgca08.bmp");
	m_pTexs[3] = I_Tex.Load(L"../../data/texture/Map.png");
}
bool	TMapObj::CreateVertexBuffer()
{
	// 화면좌표계  <-> 변환  <-> 직각좌표계
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PCT_VERTEX) * m_vVertexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vVertexList.at(0);
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	TMapObj::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(DWORD) * m_vIndexList.size();
	// 읽고쓰기권한 설정(CPU X,X, GPU 0,0)
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_vIndexList.at(0);
	HRESULT hr = TDevice::m_pd3dDevice->CreateBuffer(
		&bd, &sd, m_pIndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	TMapObj::CreateInputLayout()
{
	if (m_pShader == nullptr) return true;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// 0~8
		{ "POS",  0, DXGI_FORMAT_R32G32_FLOAT,		 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT  iNumCnt = sizeof(layout) / sizeof(layout[0]);
	m_pInputLayout = I_InputLayout.Load(m_pShader->m_pCode.Get(), layout, 3, L"PCT");
	return true;
}
void	TMapObj::Transform(TVector2 vCamera)
{
	TransformCamera(vCamera);
	TransformNDC();
	TDevice::m_pd3dContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, nullptr,
		&m_vVertexList.at(0), 0, 0);
}
TObject& TMapObj::SetTexture(TTexture* pData)
{
	m_pTexture = pData;
	return *this;
}
TObject& TMapObj::SetLayout(TInputLayout* pData)
{
	if (pData == nullptr)
	{
		m_pInputLayout = I_InputLayout.g_pInputLayout;
	}
	else
	{
		m_pInputLayout = pData;
	}
	return *this;
}
TObject& TMapObj::SetShader(TShader* pData)
{
	if (pData == nullptr)
	{
		m_pShader = I_Shader.g_pDefaultShader;
	}
	else
	{
		m_pShader = pData;
	}
	return *this;
}\
void	TMapObj::PreRender()
{
	TDevice::m_pd3dContext->PSSetShaderResources(
		0, 1, &m_pTexture->m_pTexSRV);
	TDevice::m_pd3dContext->VSSetShader(
		m_pShader->m_pVertexShader.Get(), nullptr, 0);
	TDevice::m_pd3dContext->PSSetShader(
		m_pShader->m_pPixelShader.Get(), nullptr, 0);
	TDevice::m_pd3dContext->IASetInputLayout(
		m_pInputLayout->Get());

	// 정점버퍼에서 Offsets에서 시작하여
	// Strides크기로 정점을 정점쉐이더로 전달해라.
	UINT Strides = sizeof(PCT_VERTEX);
	UINT Offsets = 0;
	TDevice::m_pd3dContext->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&Strides,
		&Offsets);
	TDevice::m_pd3dContext->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, 0);
	TDevice::m_pd3dContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void	TMapObj::Render()
{
	PreRender();
	PostRender();
}
void	TMapObj::PostRender()
{
	if (m_pIndexBuffer == nullptr)
		TDevice::m_pd3dContext->Draw(m_vVertexList.size(), 0);
	else
	{
		srand(time(NULL));
		for (int iCell = 0; iCell < m_iNumCellRow * m_iNumCellCol; iCell++)
		{
			UINT iTex = m_Cells[iCell].iTexID;// rand() % 4;
			TDevice::m_pd3dContext->PSSetShaderResources(
				0, 1, &m_pTexs[iTex]->m_pTexSRV);
			TDevice::m_pd3dContext->DrawIndexed(6, 6 * iCell, 0);
		}
	}
}

