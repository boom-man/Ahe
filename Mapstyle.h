#pragma once
#include <windows.h>

class MapStyle {
public:
    virtual void Init(float startX, float startY, float startRadius) = 0;
    virtual void Update() = 0;
    virtual void Draw(HDC hdc) = 0;
    virtual ~MapStyle() {}
};