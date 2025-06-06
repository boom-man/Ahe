#include "MapStyle0.h"

void MapStyle0::Init(float startX, float startY, float startRadius) {
    centerX = startX;
    centerY = startY;
    radius = startRadius;
}

void MapStyle0::Update() {
    // 기본 맵은 변화 없음
}

void MapStyle0::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong, HBITMAP OldBit[], int w, int h) {
    OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong);
    StretchBlt(hdc, 0, 0, 1920, 1080, hMemDC, 0, 0, 1920, 1080, SRCCOPY);
    SelectObject(hMemDC, OldBit[2]);
}