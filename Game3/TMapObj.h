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
	UINT  m_iNumRow = 2;
	UINT  m_iNumCol = 2;
	UINT  m_iNumCellRow;
	UINT  m_iNumCellCol;
	UINT  m_iNumVertex;
	UINT  m_iNumIndex;
	TVector2 m_vCellDistance;
public:
	
	std::vector<TCell>  m_Cells;
	void	SetCellCounter(UINT iRow = 10, UINT iCol = 10);
public:
	virtual void SetVertexData() override;
	virtual void SetIndexData() override;
	virtual void UpdateVertexData();
	virtual void Frame() override;
	void PostRender() override;
public:
	TMapObj(UINT iCellX, UINT iCellY);
	TMapObj(TRect rt, UINT iCellX, UINT iCellY);
	TMapObj();
};

