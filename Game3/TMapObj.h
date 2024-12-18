#pragma once
#include "TObject2D.h"
struct TCell
{
	TRect  rt;
	UINT   iTexID;
};
class TMapObj : public TObject2D
{
	TTexture* m_pTexs[4];
	UINT  m_iNumRow = 5;
	UINT  m_iNumCol = 5;
	UINT  m_iNumCellRow ;
	UINT  m_iNumCellCol;
	UINT  m_iNumVertex;
	UINT  m_iNumIndex;
public:
	std::vector<TCell>  m_Cells;

public:
	virtual void SetVertexData() override;
	virtual void SetIndexData() override;
	virtual void UpdateVertexData();
	virtual void Frame() override;
	void PostRender() override;
};

