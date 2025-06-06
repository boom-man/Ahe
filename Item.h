#pragma once
#include <windows.h>

enum ItemType {
    STAMINA,
    MAP_CHANGE1,
    MAP_CHANGE2,
    MAP_CHANGE3
};

class Item {

    int itemradius = 20;
    bool initialized = false;
    ItemType type = STAMINA;

public:
    int x, y;
    void Init(ItemType t, int tx, int ty);
    void Reset();
    void Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit);
    bool CheckCollision(float px, float py, int pradius);
    ItemType GetType() const { return type; }
    bool IsAlive() const { return initialized; }
};
