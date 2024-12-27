#include "HPBar.h"
#include "TDevice.h"

HPBar::HPBar(int maxHP, float width, float height)
    : maxHP(maxHP), currentHP(maxHP), width(width), height(height) 
{
    m_srtScreen.SetS({ 0.0f, 0.0f }, { width, height });
    SetVertexData();
    SetIndexData();
    CreateVertexBuffer();
    CreateIndexBuffer();
}

// 체력 감소
void HPBar::TakeDamage(int damage) 
{
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;
    UpdateHPBar();
}

// 체력 회복
void HPBar::Heal(int healAmount) 
{
    currentHP += healAmount;
    if (currentHP > maxHP) currentHP = maxHP;
    UpdateHPBar();
}

// HP 바 업데이트
void HPBar::UpdateHPBar()
{
    float healthRatio = static_cast<float>(currentHP) / maxHP;
    m_vScreenList[1].x = m_vScreenList[0].x + width * healthRatio;
    m_vScreenList[3].x = m_vScreenList[0].x + width * healthRatio;

    // 업데이트된 위치를 다시 정점 데이터로 변환
    SetVertexData();
    TDevice::m_pd3dContext->UpdateSubresource(
        m_pVertexBuffer.Get(), 0, nullptr, &m_vVertexList.at(0), 0, 0);
}

// 렌더링 함수
void HPBar::Render() 
{
    TObject2D::Render(); // 부모 클래스의 렌더링 호출
}
