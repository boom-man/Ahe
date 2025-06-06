#include "Spike.h"

SpikeZone::SpikeZone() {
    active = false;
    angle = 0;
    centerX = centerY = 0;
    radius = 0;
    startTime = 0;
}

void SpikeZone::Init(float a, float cx, float cy, float r, int currentFrame) {
    angle = a;
    centerX = cx;
    centerY = cy;
    radius = r;
    startTime = currentFrame;
    active = true;
}

void SpikeZone::Update(int currentFrame, Player& player) {
    if (!active) return;

    // 20초 경과 시 소멸 (프레임 기준)
    if (currentFrame - startTime > 20 * 60) {
        active = false;
        return;
    }

    // 플레이어 위치로부터 각도 계산
    float dx = player.x - centerX;
    float dy = player.y - centerY;
    float playerAngle = atan2(dy, dx);
    if (playerAngle < 0) playerAngle += 2 * PI;

    float diff = fabs(playerAngle - angle);
    if (diff > PI) diff = 2 * PI - diff;

    if (diff <= DEG2RAD(10)) {
        player.health--;
        active = false;
    }
}

void SpikeZone::Draw(HDC hdc, HBITMAP spikeImg) {
    if (!active) return;

    int drawX = (int)(cos(angle) * radius + centerX);
    int drawY = (int)(sin(angle) * radius + centerY);

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, spikeImg);

    BitBlt(hdc, drawX - 50, drawY - 50, 100, 100, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBmp);
    DeleteDC(memDC);
}