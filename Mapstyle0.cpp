#include "MapStyle0.h"

void MapStyle0::Init(float startX, float startY, float startRadius) {
    centerX = startX;
    centerY = startY;
    radius = startRadius;
}

void MapStyle0::Update() {
    // 기본 맵은 변화 없음
}

void MapStyle0::Draw(HDC hdc) {
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
}