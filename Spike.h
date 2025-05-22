#pragma once
#include <windows.h>
#include "player.h"
#include "boss.h"
#pragma comment(lib, "Msimg32.lib")
class Spike {
public:
    float x, y, speed; // 좌표 및 속도
    float dx, dy; // 방향 벡터
    int attack; // 현재 상태
    int spawncount;
    Spike();

    void Init(float startX, float startY, float dirX, float dirY, float speed, int what, int count);
    void Update(const Boss& boss,const Player& player);
    void Draw(HDC hdc);
};

class Yong {
public:
    float x, gap;
    bool attack;
    int spawncount, bitmapcount;
    int DC;

    Yong();

    void Spawn(float startX,float startGap, int spawn, int during);
    void Update();
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w, int h);
};