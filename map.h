#pragma once
#include "MapStyle.h"

class Map {
private:
    MapStyle* style; // ������ ������

public:
    Map();
    void SetStyle(MapStyle* newStyle); // ��Ÿ�� ����
    void Init(float x, float y, float r); // �ʱ�ȭ
    void Update();
    void Draw(HDC hdc);
};