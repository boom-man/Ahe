#pragma once
#include <windows.h>
class Boss {
public:
    float x, y;
    bool isGroggy;  // �׷α� ����
    int health;

    Boss();
    void Init(float startX, float startY, int startHealth);
    void Update();
    void Attack();
    void Draw(HDC hdc);
};
