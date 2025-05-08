#pragma once
#include <windows.h>
class Boss {
public:
    float x, y;
    bool isGroggy;  // 그로기 상태
    int health;

    Boss();
    void Init(float startX, float startY, int startHealth);
    void Update();
    void Attack();
    void Draw(HDC hdc);
};
