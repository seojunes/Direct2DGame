#pragma once
#include "TObject2D.h"

class HPBar : public TObject2D 
{
public:
    int maxHP;    // 최대 체력
    int currentHP; // 현재 체력
    float width;  // HP 바의 너비
    float height; // HP 바의 높이

public:
    // 생성자와 소멸자
    HPBar(int maxHP, float width, float height);
    virtual ~HPBar() {}

    // 체력 조작 메서드
    void TakeDamage(int damage);
    void Heal(int healAmount);

    // 렌더링 업데이트
    void UpdateHPBar();

    // 렌더링 함수
    virtual void Render() override;
};