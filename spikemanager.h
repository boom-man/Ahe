#pragma once
#include "Spike.h"
#include <cstdio>
#define DEG2RAD(angle) ((angle) * 3.14159265f / 180.0f)
class SpikeManager {
private:
    int Level;
public:
    static const int MAX_SPIKES = 1000;


    Spike spikes[MAX_SPIKES];
    Spike ghlwjsspikes[40];
    bool ghlwjs = false,hasOrbitPattern = false;
    Yong yong[30];
    Tracking tracking[5];

    int spikeCount, yongcount,trackcount, lineattackstart, attackstyle;
    float speed;
    int style = 0; // wide attack 전용
    SpikeManager();

    void Update(const Boss& boss, Player& player);
    void SpawnManager(HBITMAP Yong[], int w[], int h[], HDC hdc,
        Boss& boss, const Player& player, int timecount);
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]);

    // 스파이크 패턴들
    void SpawnCross(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 완
    void SpawnXShape(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 완
    void SinglePointSpikes(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY, float targetX, float targetY); // 완

    void OrbitingAttack(float centerX, float centerY, int during); // 완

    void WideAreaSpikes(HBITMAP Yong[], int w[], int h[],
        float startX, float startgap, float spawn, float during); // 완

    void PeripheralSpikes(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY); // 완


    void OrbitTimeAttack(HBITMAP Yong[], int w[], int h[],
        float centerX, float centerY, int timecount, int lineattackstart); // 완
    void TrackingOrbAttack(float centerX, float centerY); // 완
    int GetLevel() const { return Level; };
    void SetLevel(int LV) { Level = LV; };
}; 