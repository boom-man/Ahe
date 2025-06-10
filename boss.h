#pragma once
#include<iostream>
#include<cmath>
#include <windows.h>
#include "player.h"
#define PI 3.14159265f
#define DEG2RAD(angle) ((angle) * PI / 180.0f)
enum class PatternSet {
    LV1Combo1, // ��
    LV1Combo2, // ��
    LV1Combo3, // ��
    LV1Combo4, // ��

    OrbitSolo, // ��
    Level2_Combo1, // ��
    Level2_OrbitMixed, // ��
    Level2_Combo4 // ��
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

    bool isGroggy,Groggy_ing, lineattack, isPeripheralTime;  // �׷α� ����
    int health, lineattackstart;
    Boss();
    void Init(float startX, float startY, int startHealth, int LV);
    void Update(const Player& player, int timecount);
    void Attack(const Player& player,int timecount);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);
    void CenternMove(); // ���
    void LoopingMove(); // ���Ѵ� ����
    void FourMove(int timecount); // �͸�����
    void PlayerY(const Player& player); // �÷��̾����� y�� ����
    int A = 0;
    void FuckingGood(const Player& player); // ����
    int B = 0, Delay = 10;
    float dx, dy;

    void DrawGroggyDanger(HDC hdc, float cx, float cy);
};
