#include "SpikeManager.h"

int Wide_Attack_Leve2[4][8] = 
{ 
    { 0,1,2,3,2,1,0,3 },
    { 0,1,0,1,0,1,0,1 },
    { 2,3,2,3,2,3,2,3 },
    { 0,2,1,3,0,2,1,3 },
};
int style = 0;
SpikeManager::SpikeManager() {
    spikeCount = 0;
    yongcount = 0;
    speed = 0;
    Level = 2;
    lineattackstart = -1;
    attackstyle = 0;
}

void SpikeManager::Update(const Boss& boss, const Player& player) {
    // 스파이크 이동
    for (int i = 0; i < 1000; ++i) {
        if (spikes[i].attack >= 0) {
            spikes[i].Update(boss, player);
        }
    }
}

void SpikeManager::SpawnManager(HBITMAP Yong[], int w[], int h[],
    const Boss& boss, const Player& player, int timecount) {
    // 시간 기반 패턴 실행
    int pattern = timecount % 100;
    if (!boss.isGroggy) {
        if (!boss.lineattack) {
            if (pattern == 0 || pattern == 20 || pattern == 40 || pattern == 60 || pattern == 80) {
                SpawnCross(Yong,w,h, 
                    boss.x, boss.y);
            }
            if (pattern == 10 || pattern == 30 || pattern == 50 || pattern == 70 || pattern == 90) {
                SpawnXShape(Yong,w,h,
                    boss.x, boss.y);
            }
            if (pattern == 30) {
                SinglePointSpikes(Yong,w,h, 
                    boss.x, boss.y, player.x, player.y);
            }
            if (pattern == 11) {
                int a = rand() % 2;
                WideAreaSpikes(Yong,w,h, 
                    10, 30, 70, a);
            }
            if (pattern == 50) {
                PeripheralSpikes(Yong,w,h, 
                    player.x, player.y);
            }
        }
        else {
            OrbitTimeAttack(Yong,w,h, 
                boss.x, boss.y, timecount, boss.lineattackstart);
        }
    }
}


void SpikeManager::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]) {
    for (int i = 0; i < 1000; ++i) {
        if (spikes[i].attack >= 0) {
            spikes[i].Draw(hdc,hMemDC,Yong,OldBit,w,h);
        }
    }
}

void SpikeManager::SpawnCross(HBITMAP Yong[], int w[], int h[],
    float cx, float cy) {
    if (Level == 1) {
        if (spikeCount + 4 > MAX_SPIKES) spikeCount = 0; // 초과 방지
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, 0, 1, 15.0f + speed,0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, 1, 0, 15.0f + speed, 0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, 0, -1, 15.0f + speed, 0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, -1, 0, 15.0f + speed, 0,0);
    }
    else if(Level == 2) {
        if (spikeCount + 4 > MAX_SPIKES) spikeCount = 0; // 초과 방지
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 0, 1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, 0, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 0, -1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, 0, 15.0f + speed, 1, 0);
    }
}

void SpikeManager::SpawnXShape(HBITMAP Yong[], int w[], int h[],
    float cx, float cy) {
    if (Level == 1) {
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, 1, 1, 15.0f + speed, 0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, -1, 1, 15.0f + speed, 0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, 1, -1, 15.0f + speed, 0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, -1, -1, 15.0f + speed, 0,0);
    }
    else if (Level = 2){
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, 1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, 1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, -1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, -1, 15.0f + speed, 1, 0);
    }
}
void SpikeManager::SinglePointSpikes(HBITMAP Yong[], int w[], int h[],
    float cx, float cy, float targetX, float targetY) {
    if (Level == 1) {
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        float dx = targetX - cx;
        float dy = targetY - cy;
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, dx, dy, 30.0f + speed, 0,0);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, dx, dy, 30.0f + speed, 0,3);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, dx, dy, 30.0f + speed, 0,5);
        spikes[spikeCount++].Init(Yong,w,h,
            cx, cy, dx, dy, 30.0f + speed, 0,7);
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
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 5.0f + speed, 0,0);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 5.5f + speed, 0,5);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 6.0f + speed, 0,10);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 6.5f + speed, 0,15);
        }
    }
}
void SpikeManager::WideAreaSpikes(HBITMAP Yong[], int w[], int h[],
    float startX, float startgap, float spawn, float during) {
    if (Level == 1) {
        if (yongcount + 1 >= 30) {
            yongcount = 0;
            for (int i = 0; i < 30; i++) {
                yong[i].rainbowAlpha = 0;
            }
        }
        yong[yongcount++].Spawn(startX, startgap, spawn, during, 0);
        yong[yongcount].attackpoint = attackstyle;
        attackstyle = (attackstyle + 1) % 4;
    }
    else if (Level == 2) {
        if (yongcount + 8 >= 30) {
            yongcount = 0;
            for (int i = 0; i < 30; i++) {
                yong[i].rainbowAlpha = 0;
            }
        }
        for (int i = 0; i < 8; i++) {
            yong[yongcount].attackpoint = Wide_Attack_Leve2[style][i];
            yong[yongcount++].Spawn(startX, startgap, spawn, during, i * 30);
            
        }
        style = (style + 1) % 4;
    }
    else if (Level == 3) {

    }
}
void SpikeManager::PeripheralSpikes(HBITMAP Yong[], int w[], int h[],
    float targetX, float targetY) {
    struct Pos { float x, y; };
    Pos positions[4] = {
        {400, 200},
        {1400, 200},
        {400, 800},
        {1400, 800}
    };
    if (Level == 1) {
        int randsid = rand() % 2;
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        float dx1 = targetX - 400;
        float dy1 = targetY - 200;
        float dx2 = targetX - 1400;
        float dy2 = targetY - 800;
        if (randsid == 0) {
            spikes[spikeCount++].Init(Yong, w, h,
                400, 200, dx1, dy1, 10.0f + speed, 1, 0);
            spikes[spikeCount++].Init(Yong, w, h,
                1400, 200, dx2, dy1, 10.0f + speed, 1, 5);
            spikes[spikeCount++].Init(Yong, w, h,
                400, 800, dx1, dy2, 10.0f + speed, 1, 10);
            spikes[spikeCount++].Init(Yong, w, h,
                1400, 800, dx2, dy2, 10.0f + speed, 1, 15);
        }//1920, 1080
        else {
            spikes[spikeCount++].Init(Yong, w, h,
                400, 200, (1920 / 2) - 400, (1080 / 2) - 200, 15.0f + speed, 1, 0);
            spikes[spikeCount++].Init(Yong, w, h,
                1400, 200, (1920 / 2) - 1400, (1080 / 2) - 200, 15.0f + speed, 1, 5);
            spikes[spikeCount++].Init(Yong, w, h,
                400, 800, (1920 / 2) - 400, (1080 / 2) - 800, 15.0f + speed, 1, 10);
            spikes[spikeCount++].Init(Yong, w, h,
                1400, 800, (1920 / 2) - 1400, (1080 / 2) - 800, 15.0f + speed, 1, 15);
        }
    }
    else if (Level == 2) {
        for (int i = 0; i < 4; ++i) {
            float sx = positions[i].x;
            float sy = positions[i].y;

            float dx = targetX - sx;
            float dy = targetY - sy;

            float len = sqrt(dx * dx + dy * dy);
            if (len == 0) len = 1;

            dx /= len;
            dy /= len;

            // 양쪽으로 30% 기울이기 (각도 보정값을 벡터로)
            float angle = 0.3f; // 약 ±17도
            float tilted1x = dx * cos(angle) - dy * sin(angle);
            float tilted1y = dx * sin(angle) + dy * cos(angle);

            float tilted2x = dx * cos(-angle) - dy * sin(-angle);
            float tilted2y = dx * sin(-angle) + dy * cos(-angle);

            spikes[spikeCount++].Init(Yong, w, h, sx, sy, tilted1x, tilted1y, 12.0f + speed, 1, i * 5);
            spikes[spikeCount++].Init(Yong, w, h, sx, sy, tilted2x, tilted2y, 12.0f + speed, 1, i * 5 + 1);
        }
    }
}
void SpikeManager::OrbitTimeAttack(HBITMAP Yong[], int w[], int h[],
    float centerX, float centerY, int timecount, int lineattackstart) {
    static int prevStart = -2;
    static int emittedCount = 0;

    if (lineattackstart != prevStart) {
        // lineattack 시작점이 바뀌면 새 공격으로 간주
        emittedCount = 0;
        prevStart = lineattackstart;
    }

    if (spikeCount + 200 >= MAX_SPIKES) spikeCount = 0;

    const int totalSpikes = 200;
    int elapsed = timecount - lineattackstart;

    if (elapsed >= 180) {
        int burst = 3 + rand() % 5;

        for (int i = 0; i < burst && emittedCount < totalSpikes && spikeCount < MAX_SPIKES; ++i) {
            float angle = DEG2RAD(rand() % 360);
            float dirX = cos(angle);
            float dirY = sin(angle);
            float sp = 5.0f + static_cast<float>(rand() % 16);

            spikes[spikeCount++].Init(Yong,w,h,
                centerX, centerY, dirX, dirY, sp, 0, 0);
            ++emittedCount;
        }
    }
}