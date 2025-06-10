#define _CRT_SECURE_NO_WARNINGS

#include "SpikeManager.h"
int Wide_Attack_Leve2[4][8] = 
{ 
    { 0,1,2,3,2,1,0,3 },
    { 0,1,0,1,0,1,0,1 },
    { 2,3,2,3,2,3,2,3 },
    { 0,2,1,3,0,2,1,3 },
};
SpikeManager::SpikeManager() {
    spikeCount = 0;
    yongcount = 0;
    trackcount = 0;
    speed = 0;
    Level = 3;
    lineattackstart = -1;
    attackstyle = 0;
}

void SpikeManager::Update(const Boss& boss, Player& player) {
    // ������ũ �̵�
    for (int i = 0; i < 1000; ++i) {
        if (spikes[i].attack >= 0) {
            if (Level == 1) {
                spikes[i].Update(boss, player,3);
            }
            else if (Level == 2 || Level == 3) {
                spikes[i].Update(boss, player, 6);
            }
            else {
                spikes[i].Update(boss, player, 1);
            }
        }
    }
    for (int i = 0; i < 40; i++) {
        if (Level == 1) {
            ghlwjsspikes[i].Update(boss, player, 3);
        }
        if (Level == 2 || Level == 3) {
            ghlwjsspikes[i].Update(boss, player, 6);
        }
    }
    for (int i = 0; i < 5; i++) {
        tracking[i].Update(player);
    }
}

void SpikeManager::SpawnManager(HBITMAP Yong[], int w[], int h[], HDC hdc,
    Boss& boss, const Player& player, int timecount) {

    static int fullCycleStart = -600;
    static int subPatternIndex = 0;
    static PatternSet subPatterns[3];

    if (timecount - fullCycleStart >= 600) {
        // ���ο� 600�ֱ� ����
        fullCycleStart = timecount;
        subPatternIndex = 0;

        for (int i = 0; i < 3; ++i) {
            int r;
            PatternSet chosen;

            do {
                r = rand() % 5;

                if (Level == 1) {
                    switch (r) {
                    case 0: chosen = PatternSet::LV1Combo1; break;
                    case 1: chosen = PatternSet::LV1Combo2; break;
                    case 2: chosen = PatternSet::OrbitSolo; break;
                    case 3: chosen = PatternSet::LV1Combo3; break;
                    case 4: chosen = PatternSet::LV1Combo4; break;
                    }
                }
                else if (Level == 2){  // Level 2
                    switch (r) {
                    case 0: chosen = PatternSet::Level2_Combo1; break;
                    case 1: chosen = PatternSet::LV1Combo1; break;
                    case 2: chosen = PatternSet::Level2_OrbitMixed; break;
                    case 3: chosen = PatternSet::OrbitSolo; break;
                    case 4: chosen = PatternSet::Level2_Combo4; break;
                    }
                }
                else {
                    switch (r) {
                    case 0: chosen = PatternSet::Level2_Combo1; break;
                    case 1: chosen = PatternSet::LV1Combo1; break;
                    case 2: chosen = PatternSet::Level2_OrbitMixed; break;
                    case 3: chosen = PatternSet::OrbitSolo; break;
                    case 4: chosen = PatternSet::Level2_Combo4; break;
                    }
                }

                // ȸ�� ������ �̹� ������ �� ���� �ʰ� �ݺ�
            } while (hasOrbitPattern &&
                (chosen == PatternSet::OrbitSolo || chosen == PatternSet::Level2_OrbitMixed));

            // ���� Ȯ��
            subPatterns[i] = chosen;

            // �̹��� ȸ�� ������ �������� �÷��� on
            if (chosen == PatternSet::OrbitSolo || chosen == PatternSet::Level2_OrbitMixed)
                hasOrbitPattern = true;
        }
    }

    int subCycle = timecount / 200;
    boss.currentSet = subPatterns[subCycle];  // Boss::Attack()������ ��� �����ϰ�
    
    PatternSet currentSet = subPatterns[subCycle];
    if (!boss.isGroggy) {
        int localOffset = timecount % 200;
        if (!boss.lineattack) {
            switch (currentSet) {
            case PatternSet::LV1Combo1:
                if (localOffset % 30 == 0)
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                else if (localOffset % 30 == 15 )
                    SinglePointSpikes(Yong, w, h, boss.x, boss.y, player.x, player.y);
                if (localOffset == 90 || localOffset == 150)
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                if (localOffset == 1)
                    WideAreaSpikes(Yong, w, h, 10, 30, 70, rand() % 2);
                break;

            case PatternSet::LV1Combo2:
                if (localOffset % 30 == 0)
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                else if (localOffset % 30 == 15)
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                if (localOffset % 90 == 45)
                    SinglePointSpikes(Yong, w, h, boss.x, boss.y, player.x, player.y);
                if (localOffset % 180 == 120)
                    PeripheralSpikes(Yong, w, h, player.x, player.y);
                break;

            case PatternSet::LV1Combo3:
                if (localOffset % 30 == 0) {
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                }
                if (localOffset == 0 && !ghlwjs)
                    OrbitingAttack(1920 / 2, 1080 / 2, 190);
                if (localOffset % 60 == 0)
                    PeripheralSpikes(Yong, w, h, player.x, player.y);
                break;
            case PatternSet::LV1Combo4:
                if (localOffset % 30 == 0)
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                else if (localOffset % 30 == 15)
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                if (localOffset == 1) {
                    TrackingOrbAttack(boss.x, boss.y);
                }
                if (localOffset == 10)
                    WideAreaSpikes(Yong, w, h, 10, 30, 70, rand() % 2);
                if (localOffset % 90 == 45)
                    SinglePointSpikes(Yong, w, h, boss.x, boss.y, player.x, player.y);
                break;

            case PatternSet::OrbitSolo:
                if (localOffset == 0 && !ghlwjs)
                    OrbitingAttack(1920 / 2, 1080 / 2, 190);
                if (localOffset % 30 == 10) {
                    PeripheralSpikes(Yong, w, h, player.x, player.y);
                }
                break;

            case PatternSet::Level2_Combo1:
                if (localOffset % 30 == 0)
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                else if (localOffset % 30 == 15)
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                if (localOffset % 50 == 25)
                    SinglePointSpikes(Yong, w, h, boss.x, boss.y, player.x, player.y);
                if (localOffset % 30 == 20) {
                    PeripheralSpikes(Yong, w, h, player.x, player.y);
                }
                break;

            case PatternSet::Level2_OrbitMixed:
                if (localOffset == 0 && !ghlwjs)
                    OrbitingAttack(1920 / 2, 1080 / 2, 190);
                if (localOffset == 20 || localOffset == 60 || localOffset == 100)
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                if (localOffset == 40 || localOffset == 80 || localOffset == 120)
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                break;
            case PatternSet::Level2_Combo4:
                if (localOffset % 30 == 0) {
                    SpawnCross(Yong, w, h, boss.x, boss.y);
                    SpawnXShape(Yong, w, h, boss.x, boss.y);
                }
                if (localOffset % 90 == 45)
                    PeripheralSpikes(Yong, w, h, player.x, player.y);
                if (localOffset == 0 || localOffset == 50) {
                    TrackingOrbAttack(boss.x, boss.y);
                }
                break;
            }
        }
        if (boss.go) {
            OrbitTimeAttack(Yong, w, h,
                boss.x, boss.y, timecount, boss.lineattackstart);
            boss.page++;
            hasOrbitPattern = true;
        }
    }
}



void SpikeManager::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]) {
    for (int i = 0; i < 1000; ++i) {
        if (spikes[i].attack >= 0) {
            spikes[i].Draw(hdc, hMemDC, Yong, OldBit, w, h);
        }
    }
    for (int i = 0; i < 40; i++) {
        if (ghlwjsspikes[i].attack >= 0) {
            ghlwjsspikes[i].Draw(hdc, hMemDC, Yong, OldBit, w, h);
        }
    }
    for (int i = 0; i < 5; i++) {
        tracking[i].Draw(hdc);
    }
}

void SpikeManager::SpawnCross(HBITMAP Yong[], int w[], int h[],
    float cx, float cy) {
    if (Level == 1) {
        if (spikeCount + 4 > MAX_SPIKES) spikeCount = 0; // �ʰ� ����
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
        if (spikeCount + 4 > MAX_SPIKES) spikeCount = 0; // �ʰ� ����
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 0, 1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, 0, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 0, -1, 15.0f + speed, 1, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, 0, 15.0f + speed, 1, 0);
    }
    else {
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 0, 1, 15.0f + speed, 1,  1);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, 1, 15.0f + speed, 1,  2);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, 0, 15.0f + speed, 1,  3);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, -1, 15.0f + speed, 1,  4);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 0, -1, 15.0f + speed, 1,  5);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, -1, 15.0f + speed, 1,  6);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, 0, 15.0f + speed, 1,  7);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, 1, 15.0f + speed, 1,  8);
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
    else if (Level == 2){
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
    else {
        if (spikeCount + 4 >= MAX_SPIKES) spikeCount = 0;
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, 1, 30.0f + speed, 0, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, 1, 30.0f + speed, 0, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, 1, -1, 30.0f + speed, 0, 0);
        spikes[spikeCount++].Init(Yong, w, h,
            cx, cy, -1, -1, 30.0f + speed, 0, 0);
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
    if (Level == 2 || Level == 3) {
        if (spikeCount + 12 >= MAX_SPIKES) spikeCount = 0;
        // ���� ���� ����
        float dirX = targetX - cx;
        float dirY = targetY - cy;
        // ������ 0�� ��� ����
        float len = sqrt(dirX * dirX + dirY * dirY);
        if (len == 0.0f) len = 1.0f;
        dirX /= len;
        dirY /= len;
        // ���� ���� ���ϱ�
        float baseAngle = atan2(dirY, dirX);
        // -15��, 0��, +15�� �� ����
        float angles[3] = {
            baseAngle - DEG2RAD(30.0f),
            baseAngle,
            baseAngle + DEG2RAD(30.0f)
        };
        for (int i = 0; i < 3; ++i) {
            float dx = cos(angles[i]);
            float dy = sin(angles[i]);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 25.0f + speed, 0,0);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 25.5f + speed, 0,5);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 26.0f + speed, 0,10);
            spikes[spikeCount++].Init(Yong,w,h,
                cx, cy, dx, dy, 26.5f + speed, 0,15);
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
            yong[yongcount++].Spawn(startX, startgap, spawn, during, i * 70);
        }
        style = (style + 1) % 4;
    }
    else if (Level == 3) {
        if (yongcount + 8 >= 30) {
            yongcount = 0;
            for (int i = 0; i < 30; i++) {
                yong[i].rainbowAlpha = 0;
            }
        }
        for (int i = 0; i < 8; i++) {
            yong[yongcount].attackpoint = Wide_Attack_Leve2[3 - style][i];
            yong[yongcount++].Spawn(startX, startgap, spawn, during, i * 70);
        }
        style = (style + 1) % 4;
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
    else if (Level == 2 || Level == 3) {
        for (int i = 0; i < 4; ++i) {
            float sx = positions[i].x;
            float sy = positions[i].y;

            float dx = targetX - sx;
            float dy = targetY - sy;

            float len = sqrt(dx * dx + dy * dy);
            if (len == 0) len = 1;

            dx /= len;
            dy /= len;

            // �������� 30% ����̱� (���� �������� ���ͷ�)
            float angle = 0.3f; // �� ��17��
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
        // lineattack �������� �ٲ�� �� �������� ����
        emittedCount = 0;
        prevStart = lineattackstart;
    }

    if (spikeCount + 100 >= MAX_SPIKES) spikeCount = 0;

    const int totalSpikes = 100;

    int burst = 3 + rand() % 5;
    for (int i = 0; i < burst && emittedCount < totalSpikes && spikeCount < MAX_SPIKES; ++i) {
        float angle = DEG2RAD(rand() % 360);
        float dirX = cos(angle);
        float dirY = sin(angle);
        float sp = 5.0f + static_cast<float>(rand() % 16);
        spikes[spikeCount++].Init(Yong, w, h,
            centerX, centerY, dirX, dirY, sp, 0, 0);
        ++emittedCount;
    }
    
}
void SpikeManager::OrbitingAttack(float centerX, float centerY,int during) {
    const float spacing = 50.0f;           // �� ���� ������ ����
    const float speed = 0.0125f;             // ȸ�� �ӵ� (�����Ӵ� ����)
    const int spawnDelay = 30;              // ��� ���� (�ʿ�� ����)
    int Count = 0;
    for (int i = 1; i <= 10; ++i) {
        float r = i * spacing;
        if (Level == 2 || Level == 3) {
            for (int j = 0; j < 4; ++j) {
                float angle = j * (PI / 2); // 0, ��/2, ��, 3��/2
                if (Count >= 40) return;
                ghlwjsspikes[Count].radiusInit(
                    centerX, centerY,  // �߽� ��ǥ
                    2,               // attack Ÿ��: ȸ��
                    speed,
                    spawnDelay,
                    angle,
                    r,
                    during
                );

                Count++;
            }
        }
        else if (Level == 1) {
            for (int j = 0; j < 2; ++j) {
                float angle = j * PI; // 0, ��/2, ��, 3��/2
                if (Count >= 40) return;
                ghlwjsspikes[Count].radiusInit(
                    centerX, centerY,  // �߽� ��ǥ
                    2,               // attack Ÿ��: ȸ��
                    speed,
                    spawnDelay,
                    angle,
                    r,
                    during
                );

                Count++;
            }
        }
    }
}
void SpikeManager::TrackingOrbAttack(float centerX, float centerY) {
    // trackcount�� ��� ������� �����ϰ� �ʱ�ȭ �� ī��Ʈ ����
    if (Level == 1) {
        if (trackcount + 1 >= 5) {
            trackcount = 0;
        }
        else {
            trackcount++;
        }
        // tracking[trackcount]�� �ʱ�ȭ�ؼ� ���� ���� Ȱ��ȭ
        tracking[trackcount].Init(centerX, centerY, 0, 7.0f /*�ӵ�*/, 50 /*���ӽð�*/);
    }
    else if (Level == 2) {
        if (trackcount + 2 >= 5) {
            trackcount = 0;
        }
        // tracking[trackcount]�� �ʱ�ȭ�ؼ� ���� ���� Ȱ��ȭ
        tracking[trackcount++].Init(centerX, centerY, 0, 7.0f /*�ӵ�*/, 50 /*���ӽð�*/);
        tracking[trackcount++].Init(centerX, centerY, 30, 7.0f /*�ӵ�*/, 50 /*���ӽð�*/);
    }
    else {
        if (trackcount + 1 >= 5) {
            trackcount = 0;
        }
        else {
            trackcount++;
        }
        // tracking[trackcount]�� �ʱ�ȭ�ؼ� ���� ���� Ȱ��ȭ
        tracking[trackcount].Init(centerX, centerY, 0, 10.0f /*�ӵ�*/, 30 /*���ӽð�*/);
    }
}
void SpikeManager::Reset() {
    spikeCount = 0;
    for (int i = 0; i < 1000; i++) {
        if (spikes[i].attack >= 0) {
            spikes[i].attack = -1;
        }
    }
    yongcount = 0;
    for (int i = 0; i < 30; i++) {
        yong[i].attack = false;
        yong[i].rainbowAlpha = 0;
    }
    trackcount = 0;
    for (int i = 0; i < 5; i++) {
        tracking[i].active = false;
    }
    speed = 0;
    Level = 3;
    lineattackstart = -1;
    attackstyle = 0;
    for (int i = 0; i < 30; i++) {
        yong[i].Reset();  // Spike Ŭ������ Reset() �޼��� ���� �ʿ�
    }
}