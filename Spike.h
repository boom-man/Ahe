#pragma once
#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#include "player.h"
#include "boss.h"
#pragma comment(lib, "Msimg32.lib")
#define PI 3.14159265f
#define DEG2RAD(angle) ((angle) * PI / 180.0f)
class Spike {
public:
    float x, y, speed; // ��ǥ �� �ӵ�
    float dx, dy; // ���� ����
    float cx, radius,angle; // ��Ʋ��Ʋ �ϴ°� and ȸ���Ҷ� radius
    int attack; // ���� ����
    int spawncount, bitmapcount = 0, during;// �� �� �� ����
    bool attack4;
    Spike();
    bool check(Player& player);
    void Init(HBITMAP Yong[], int w[], int h[],
        float startX, float startY, float dirX, float dirY, float speed, int what, int count);
    void radiusInit(float centerX, float centerY, int what, float sp, int count, float angleInput, float radiusInput,int during);
    void Update(const Boss& boss,Player& player,int i);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);
};

class Yong {
public:
    float x,y, gap;
    bool attack;
    int spawncount, bitmapcount, Delay;
    int BangHyang, attackpoint;
    int rainbowAlpha = 0;
    RECT attackRect;

    Yong();
    bool CheckCollision(const RECT& rect, float px, float py);
    void Spawn(float startX,float startGap, int spawn, int during,int Dlay);
    void Update(Player& player, int w[], int h[]);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);
    void DrawRainbowLines(HDC hdc, int alpha, int attackpoint);
};

class Tracking {
public:
    bool active;
    float centerX, centerY, speed; // �˵� �߽�
    int spawncount, duration;        // ���� ������ �ð�

    Tracking();
    void Init(float startX, float startY, int spawnFrame, float speed, int duration);
    void Update( Player& player);
    void Draw(HDC hdc);
};