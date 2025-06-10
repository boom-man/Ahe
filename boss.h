#pragma once
#include<iostream>
#include<cmath>
#include <windows.h>
#include "player.h"
#define PI 3.14159265f
#define DEG2RAD(angle) ((angle) * PI / 180.0f)
enum class PatternSet {
    LV1Combo1, // 완
    LV1Combo2, // 완
    LV1Combo3, // 완
    LV1Combo4, // 완

    OrbitSolo, // 완
    Level2_Combo1, // 완
    Level2_OrbitMixed, // 완
    Level2_Combo4 // 완
};
class Boss {
public:
    float x, y, speed, groggyAngle;
    HBITMAP hBitmap;
    HDC hMemDC;
    PatternSet currentSet;
    int bitcount, qcenterX, qcenterY;

    int page, Level, groggytime;


    bool debug;

    bool isGroggy,Groggy_ing, lineattack, isPeripheralTime;  // 그로기 상태
    int health, lineattackstart;
    Boss();
    void Init(float startX, float startY, int startHealth, int LV);
    void Update(const Player& player, int timecount);
    void Attack(const Player& player,int timecount);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);
    void CenternMove(); // 가운데
    void LoopingMove(); // 무한대 궤적
    void FourMove(int timecount); // 와리가리
    void PlayerY(const Player& player); // 플레이어한테 y축 고정
    int A = 0;
    void FuckingGood(const Player& player); // 돌진
    int B = 0, Delay = 10;
    float dx, dy;

    void DrawGroggyDanger(HDC hdc, float cx, float cy);
};
