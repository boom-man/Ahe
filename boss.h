#pragma once
#include <windows.h>
#include "player.h"
class Boss {
public:
    float x, y, speed;
    bool isGroggy;  // 그로기 상태
    int health;

    Boss();
    void Init(float startX, float startY, int startHealth);
    void Update(const Player&player);
    void Attack();
    void Draw(HDC hdc);
};
