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

// ü�� ����
void HPBar::TakeDamage(int damage) 
{
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;
    UpdateHPBar();
}

// ü�� ȸ��
void HPBar::Heal(int healAmount) 
{
    currentHP += healAmount;
    if (currentHP > maxHP) currentHP = maxHP;
    UpdateHPBar();
}

// HP �� ������Ʈ
void HPBar::UpdateHPBar()
{
    float healthRatio = static_cast<float>(currentHP) / maxHP;
    m_vScreenList[1].x = m_vScreenList[0].x + width * healthRatio;
    m_vScreenList[3].x = m_vScreenList[0].x + width * healthRatio;

    // ������Ʈ�� ��ġ�� �ٽ� ���� �����ͷ� ��ȯ
    SetVertexData();
    TDevice::m_pd3dContext->UpdateSubresource(
        m_pVertexBuffer.Get(), 0, nullptr, &m_vVertexList.at(0), 0, 0);
}

// ������ �Լ�
void HPBar::Render() 
{
    TObject2D::Render(); // �θ� Ŭ������ ������ ȣ��
}
