#pragma once
#include <windows.h>
#include "player.h"
#include "boss.h"
#pragma comment(lib, "Msimg32.lib")
#define PI 3.14159265f
#define DEG2RAD(angle) ((angle) * PI / 180.0f)
class Spike {
public:
    float x, y, speed; // 좌표 및 속도
    float dx, dy; // 방향 벡터
    float cx; // 꿈틀꿈틀 하는거 
    int attack; // 현재 상태
    int spawncount, bitmapcount = 0;// 몇 초 뒤 생성
    Spike();

    void Init(HBITMAP Yong[], int w[], int h[],
        float startX, float startY, float dirX, float dirY, float speed, int what, int count);
    void Update(const Boss& boss,const Player& player);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);
};

class Yong {
public:
    float x,y, gap;
    bool attack;
    int spawncount, bitmapcount, Delay;
    int BangHyang, attackpoint;
    int rainbowAlpha = 0;
    Yong();

    void Spawn(float startX,float startGap, int spawn, int during,int Dlay);
    void Update();
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);
    void DrawRainbowLines(HDC hdc, int alpha, int attackpoint);
};

class SpikeZone {
public:
    bool active;
    float angle;            // 중심 각도 (라디안)
    float centerX, centerY; // 궤도 중심
    float radius;           // 궤도 반지름
    int startTime;          // 생성 프레임 시간

    SpikeZone();
    void Init(float angle, float centerX, float centerY, float radius, int currentFrame);
    void Update(int currentFrame, Player& player);
    void Draw(HDC hdc, HBITMAP spikeImg);
};