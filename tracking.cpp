#include <windows.h>
#include "Spike.h"
#include <cmath>

Tracking::Tracking() {
    active = false;
    centerX = centerY = 0.0f;
    spawncount = 0;
    speed = 1.5f;         // 🟨 기본 속도 (수정 가능)
    duration = 300;       // 🟨 5초 유지 (60FPS 기준)
}

void Tracking::Init(float startX, float startY, int spawnFrame, float speedVal, int durationVal) {
    active = true;
    centerX = startX;
    centerY = startY;
    spawncount = spawnFrame;

    speed = speedVal;         // ⚙️ 속도 (예: 1.5f)
    duration = durationVal;   // ⚙️ 유지 시간 (프레임 단위)

    // radius는 현재 사용하지 않음
}

void Tracking::Update( Player& player) {
    if (!active) return;

    if (spawncount > 0) {
        spawncount--;
        return;
    }

    // 유지 시간 체크
    if (duration <= 0) {
        active = false;
        return;
    }

    // 플레이어 위치로 향하는 방향 벡터 계산
    float dx = player.x - centerX;
    float dy = player.y - centerY;
    float dist = std::sqrt(dx * dx + dy * dy);

    float playerRadius = 10.0f;   // 플레이어 반지름 (지름 20)
    float orbRadius = 100.0f;     // 트래킹 오브젝트 반지름 (지름 200)
    
    if (dist < playerRadius + orbRadius) {
        // 충돌: 플레이어 체력 감소
        if (player.delay <= 0){
            player.health--;
            player.delay = 10;
        }
        return;
    }

    if (dist > 1.0f) {
        dx /= dist;
        dy /= dist;

        centerX += dx * speed;
        centerY += dy * speed;
    }
}

void Tracking::Draw(HDC hdc) {
    if (!active) return;

    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    int size = 200;
    SolidBrush brush(Color(128, 150, 150, 255));  // 알파=128(절반 투명), 노란색

    graphics.FillEllipse(
        &brush,
        (int)(centerX - size / 2),
        (int)(centerY - size / 2),
        size, size
    );
}