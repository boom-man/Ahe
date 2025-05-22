#pragma once
#include "Spike.h"
#include "Boss.h"
#include "Player.h"
#define DEG2RAD(angle) ((angle) * 3.14159265f / 180.0f)
class SpikeManager {
public:
    static const int MAX_SPIKES = 1000;

    Spike spikes[MAX_SPIKES];
    Yong yong;
    int spikeCount;
    float speed;
    int Level;
    SpikeManager();

    void Update(const Boss& boss, const Player& player);
    void SpawnManager(const Boss& boss, const Player& player, int timecount);
    void Draw(HDC hdc);

    // 스파이크 패턴들
    void SpawnCross(float centerX, float centerY);
    void SpawnXShape(float centerX, float centerY);
    void SinglePointSpikes(float centerX, float centerY, float targetX, float targetY);

    void OrbitingAttack(float centerX, float centerY);

    void WideAreaSpikes(float startX, float startgap, float spawn, float during);

    void PeripheralSpikes(float centerX, float centerY);
    void RandomOrbitSpikes(float centerX, float centerY);
    void OrbitTimeAttack(float centerX, float centerY);
    void TrackingOrbAttack(float centerX, float centerY);
}; 