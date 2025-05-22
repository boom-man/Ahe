#pragma once
#include<iostream>
#include<cmath>
#include <windows.h>
#include "player.h"
class Boss {
public:
    float x, y, speed;
    HBITMAP hBitmap;
    HDC hMemDC;
    int bitcount;

    bool isGroggy, patterning;  // 그로기 상태
    int health;
    Boss();
    void Init(float startX, float startY, int startHealth);
    void Update(const Player&player,int timecount);
    void Attack(int timecount);
    void Draw(HDC hdc);
    void CrossSpikes();
    void XShapedSpikes();
    void SinglePointSpikes();
    void OrbitingAttack();
    void WideAreaSpikes();
    void PeripheralSpikes();
    void RandomOrbitSpikes();
    void OrbitTimeAttack();
    void TrackingOrbAttack();
};
