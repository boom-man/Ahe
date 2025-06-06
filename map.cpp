#include "Map.h"

Map::Map() : style(nullptr) {}

void Map::SetStyle(MapStyle* newStyle) {
    style = newStyle;
}

void Map::Init(float x, float y, float r) {
    if (style)
        style->Init(x, y, r);
}

void Map::Update() {
    if (style)
        style->Update();
}

void Map::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong, HBITMAP OldBit[], int w, int h) {
    if (style)
        style->Draw(hdc, hMemDC,Yong,OldBit, w,h);
}