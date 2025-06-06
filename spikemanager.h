#pragma once
#include "Spike.h"
#include "Boss.h"
#include "Player.h"
#define DEG2RAD(angle) ((angle) * 3.14159265f / 180.0f)
class SpikeManager {
public:
    static const int MAX_SPIKES = 1000;

    Spike spikes[MAX_SPIKES];
    Yong yong[30];
    SpikeZone spikezone[10];
    int spikeCount, yongcount, lineattackstart, attackstyle;
    float speed;
    int Level;
    SpikeManager();

    void Update(const Boss& boss, const Player& player);
    void SpawnManager(HBITMAP Yong[], int w[], int h[],
        const Boss& boss, const Player& player, int timecount);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);

    // 스파이크 패턴들
    void SpawnCross(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 완
    void SpawnXShape(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 완
    void SinglePointSpikes(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY, float targetX, float targetY); // 완

    void OrbitingAttack(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 미완

    void WideAreaSpikes(HBITMAP Yong[], int w[], int h[],
        float startX, float startgap, float spawn, float during); // 완

    void PeripheralSpikes(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 완
    void RandomOrbitSpikes(HBITMAP Yong[], int w[], int h[],
        float angle, float centerX, float centerY, float radius, int currentFrame); // 미완
    void OrbitTimeAttack(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY, int timecount, int lineattackstart); // 완
    void TrackingOrbAttack(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 미완
}; 