#pragma once
#include "UMaterialInterface.h"
#include "TTexture.h"
#include "TInputLayout.h"
class UMaterial : public UMaterialInterface
{
public:
	TShader* m_pShader = nullptr;
	TTexture* m_pTexture = nullptr;
	TInputLayout* m_pInputLayout = nullptr;
public:
	UMaterial() {}
	UMaterial(std::wstring hlsl, std::wstring texname) {
		Load(hlsl, texname);
	}
	virtual ~UMaterial() {}
public:
	virtual void Load(std::wstring shaderfilename, std::wstring texturefilename);
	virtual void SetShader(TShader* pShader = nullptr);
	virtual void SetTexture(TTexture* pTexture = nullptr);
	virtual void SetInputLayout(TInputLayout* pInputLayout = nullptr);
};
