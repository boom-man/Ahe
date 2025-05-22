#include "SpikeManager.h"

SpikeManager::SpikeManager() {
    spikeCount = 0;
    speed = 0;
    Level = 1;
}

void SpikeManager::Update(const Boss& boss, const Player& player) {
    // 스파이크 이동
    for (int i = 0; i < 1000; ++i) {
        if (spikes[i].attack >= 0) {
            spikes[i].Update(boss, player);
        }
    }
    yong.Update();
    
}

void SpikeManager::SpawnManager(const Boss& boss, const Player& player, int timecount) {
    // 시간 기반 패턴 실행
    if (timecount % 10 == 0) {
        SpawnCross(boss.x, boss.y);
    }
    if (timecount % 10 == 5) {
        SpawnXShape(boss.x, boss.y);
    }
    if (timecount % 10 == 0) {
        SinglePointSpikes(boss.x, boss.y, player.x, player.y);
    }
    if (timecount % 10 == 5) {
        WideAreaSpikes(100, 300, 0, 100);
    }

}


void SpikeManager::Draw(HDC hdc) {
    for (int i = 0; i < 1000; ++i) {
        if (spikes[i].attack >= 0) {
            spikes[i].Draw(hdc);
        }
    }
}

void SpikeManager::SpawnCross(float cx, float cy) {
    if (Level == 1) {
        if (spikeCount + 4 > MAX_SPIKES) spikeCount = 0; // 초과 방지
        spikes[spikeCount++].Init(cx, cy, 0, 1, 5.0f + speed,0,0);
        spikes[spikeCount++].Init(cx, cy, 1, 0, 5.0f + speed, 0,0);
        spikes[spikeCount++].Init(cx, cy, 0, -1, 5.0f + speed, 0,0);
        spikes[spikeCount++].Init(cx, cy, -1, 0, 5.0f + speed, 0,0);
    }
}

void SpikeManager::SpawnXShape(float cx, float cy) {
    if (Level == 1) {
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        spikes[spikeCount++].Init(cx, cy, 1, 1, 5.0f + speed, 0,0);
        spikes[spikeCount++].Init(cx, cy, -1, 1, 5.0f + speed, 0,0);
        spikes[spikeCount++].Init(cx, cy, 1, -1, 5.0f + speed, 0,0);
        spikes[spikeCount++].Init(cx, cy, -1, -1, 5.0f + speed, 0,0);
    }
}
void SpikeManager::SinglePointSpikes(float cx, float cy, float targetX, float targetY) {
    if (Level == 1) {
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        float dx = targetX - cx;
        float dy = targetY - cy;
        spikes[spikeCount++].Init(cx, cy, dx, dy, 30.0f + speed, 0,0);
        spikes[spikeCount++].Init(cx, cy, dx, dy, 30.0f + speed, 0,3);
        spikes[spikeCount++].Init(cx, cy, dx, dy, 30.0f + speed, 0,5);
        spikes[spikeCount++].Init(cx, cy, dx, dy, 30.0f + speed, 0,7);
    }
    if (Level == 2) {
        if (spikeCount + 12 >= MAX_SPIKES) spikeCount = 0;
        // 기준 방향 벡터
        float dirX = targetX - cx;
        float dirY = targetY - cy;
        // 방향이 0일 경우 방지
        float len = sqrt(dirX * dirX + dirY * dirY);
        if (len == 0.0f) len = 1.0f;
        dirX /= len;
        dirY /= len;
        // 기준 각도 구하기
        float baseAngle = atan2(dirY, dirX);
        // -15도, 0도, +15도 세 방향
        float angles[3] = {
            baseAngle - DEG2RAD(15.0f),
            baseAngle,
            baseAngle + DEG2RAD(15.0f)
        };
        for (int i = 0; i < 3; ++i) {
            float dx = cos(angles[i]);
            float dy = sin(angles[i]);
            spikes[spikeCount++].Init(cx, cy, dx, dy, 5.0f + speed, 0,0);
            spikes[spikeCount++].Init(cx, cy, dx, dy, 5.5f + speed, 0,5);
            spikes[spikeCount++].Init(cx, cy, dx, dy, 6.0f + speed, 0,10);
            spikes[spikeCount++].Init(cx, cy, dx, dy, 6.5f + speed, 0,15);
        }
    }
}
void SpikeManager::WideAreaSpikes(float startX, float startgap, float spawn, float during) {
    if (Level == 1) {
        yong.Spawn(startX, startgap, spawn, during);
    }
}
