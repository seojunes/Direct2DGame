#pragma once
#include "TObject2D.h"
class TItem :public TObject2D
{
public:
	void HitOverlap(TObject* pObj, THitResult hRes) override;
	void Frame() override;
	void SetData(vector<vector<RECT>> SpriteList);
	virtual void SetVertexData() override;

	void setMonPos(TVector2 pos);

	TVector2 m_vResponPos;

public: 
	/*TItem(TVector2 pos)
	{
		m_vResponPos = pos;
	}*/

public:
	std::vector<RECT> m_rtItemFrames; // ��Ƽ����Ż �ִϸ��̼� ������ ����Ʈ
	UINT m_iItemFrame = 0;            // ���� ���� �ִϸ��̼� ������

	float m_fItemFrameTime = 0.5f;
	float m_fCurrentTime = 0.0f;
};

