#pragma once
#include "MapStyle.h"

class MapStyle0 : public MapStyle {
private:
    float centerX, centerY, radius;
public:
    void Init(float startX, float startY, float startRadius) override;
    void Update() override;
    void Draw(HDC hdc, HDC hMemDC, HBITMAP Yong, HBITMAP OldBit[], int w, int h) override;
};