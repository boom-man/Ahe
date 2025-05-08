#pragma once
#include "MapStyle.h"

class Map {
private:
    MapStyle* style; // 다형성 포인터

public:
    Map();
    void SetStyle(MapStyle* newStyle); // 스타일 설정
    void Init(float x, float y, float r); // 초기화
    void Update();
    void Draw(HDC hdc);
};