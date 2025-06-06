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
    int page, Level;

    bool isGroggy,Groggy_ing, lineattack;  // 그로기 상태
    int health, lineattackstart;
    Boss();
    void Init(float startX, float startY, int startHealth, int LV);
    void Update(const Player&player,int timecount);
    void Attack(int timecount);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong, HBITMAP OldBit[], int w, int h);
    void CenternMove();
    void LoopingMove();
    void FourMove(int timecount);
};
