#include "MapStyle0.h"

void MapStyle0::Init(float startX, float startY, float startRadius) {
    centerX = startX;
    centerY = startY;
    radius = startRadius;
}

void MapStyle0::Update() {
    // �⺻ ���� ��ȭ ����
}

void MapStyle0::Draw(HDC hdc) {
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
}