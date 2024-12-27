#pragma once
#include "TObject2D.h"

class HPBar : public TObject2D 
{
public:
    int maxHP;    // �ִ� ü��
    int currentHP; // ���� ü��
    float width;  // HP ���� �ʺ�
    float height; // HP ���� ����

public:
    // �����ڿ� �Ҹ���
    HPBar(int maxHP, float width, float height);
    virtual ~HPBar() {}

    // ü�� ���� �޼���
    void TakeDamage(int damage);
    void Heal(int healAmount);

    // ������ ������Ʈ
    void UpdateHPBar();

    // ������ �Լ�
    virtual void Render() override;
};