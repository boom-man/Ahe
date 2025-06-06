#include "Item.h"
#include <cmath>
#include <cstdlib>

void Item::Init(ItemType t, int tx, int ty) {
    x = tx;//580;//430
    y = ty;//500;
    type = t;
    initialized = true;
}

void Item::Reset() {
    initialized = false;
    x = -100;
    y = -100;
}

bool Item::CheckCollision(float px, float py, int pradius) {
    float dx = x - px;
    float dy = y - py;
    float distance = sqrt(dx * dx + dy * dy);

    // ³Ë³ËÇÑ ¹üÀ§·Î ¼³Á¤
    int collisionRadius = itemradius + pradius + 10;

    if (distance < collisionRadius) {
        // OutputDebugString(L"[Ãæµ¹] Ãæµ¹ °¨ÁöµÊ!\n");
        return true;
    }

    return false;
}


//void Item::Draw(HDC hdc, HBITMAP staminaBmp, HBITMAP mapChangeBmp) {
//    if (!initialized) return;
//
//    HDC memDC = CreateCompatibleDC(hdc);
//    HBITMAP selectedBmp = (type == STAMINA) ? staminaBmp : mapChangeBmp;
//    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, selectedBmp);
//
//    TransparentBlt(hdc, x - itemradius, y - itemradius,
//        itemradius * 2, itemradius * 2, memDC, 0, 0, 100, 100, RGB(0, 0, 0));
//
//    SelectObject(memDC, oldBmp);
//    DeleteDC(memDC);
//}

void Item::Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit) {
    if (!initialized) return;

    HDC hdcmem = CreateCompatibleDC(hdc);
    SelectObject(hdcmem, hbit);
    TransparentBlt(hdc, x, y, 40, 40, hdcmem, 0, 0, 100, 100, RGB(0, 0, 0));
    DeleteDC(hdcmem);
}
