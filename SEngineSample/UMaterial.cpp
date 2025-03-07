#include "UMaterial.h"
void UMaterial::Load(std::wstring shaderfilename,
	std::wstring texturefilename)
{
	m_pShader = I_Shader.Load(shaderfilename);
	m_pTexture = I_Tex.Load(texturefilename);
	m_pInputLayout = I_InputLayout.GetPtr(L"PNCT");
}
void UMaterial::SetShader(TShader* pShader)
{
	m_pShader = pShader;
}
void UMaterial::SetTexture(TTexture* pTexture)
{
	m_pTexture = pTexture;
}