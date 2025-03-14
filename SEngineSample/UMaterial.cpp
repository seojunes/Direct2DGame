#include "UMaterial.h"
// 싱글턴을 이용해서 데이터 로드
void UMaterial::Load(std::wstring shaderfilename, std::wstring texturefilename)
{
	m_pShader = I_Shader.Load(shaderfilename);
	m_pTexture = I_Tex.Load(texturefilename);
	m_pInputLayout = I_InputLayout.GetPtr(L"PNCT");
}
// Shader와 Texture를 바꿔주고 싶을 때 사용 // 현재는 사용하지 x
void UMaterial::SetShader(TShader* pShader)
{
	m_pShader = pShader;
}
void UMaterial::SetTexture(TTexture* pTexture)
{
	m_pTexture = pTexture;
}