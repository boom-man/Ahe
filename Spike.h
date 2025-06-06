#pragma once
#include <windows.h>
#include "player.h"
#include "boss.h"
#pragma comment(lib, "Msimg32.lib")
#define PI 3.14159265f
#define DEG2RAD(angle) ((angle) * PI / 180.0f)
class Spike {
public:
    float x, y, speed; // ��ǥ �� �ӵ�
    float dx, dy; // ���� ����
    float cx; // ��Ʋ��Ʋ �ϴ°� 
    int attack; // ���� ����
    int spawncount, bitmapcount = 0;// �� �� �� ����
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
    float angle;            // �߽� ���� (����)
    float centerX, centerY; // �˵� �߽�
    float radius;           // �˵� ������
    int startTime;          // ���� ������ �ð�

    SpikeZone();
    void Init(float angle, float centerX, float centerY, float radius, int currentFrame);
    void Update(int currentFrame, Player& player);
    void Draw(HDC hdc, HBITMAP spikeImg);
};