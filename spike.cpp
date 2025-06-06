#include <Windows.h>
#include "Spike.h"
Spike::Spike() : x(0), y(0), attack(-1), speed(5.0f), spawncount(0), cx(-1.0f) {}

void Spike::Init(HBITMAP Yong[], int w[], int h[],
    float startX, float startY, float dirX, float dirY, float sp, int what, int count) {

    x = startX;
    y = startY;
    attack = what;
    speed = sp;
    spawncount = count;
    cx = -1.0f;

    // 방향 벡터 정규화
    float length = sqrt(dirX * dirX + dirY * dirY);
    if (length != 0.0f) {
        dx = dirX / length;
        dy = dirY / length;
    }
    else {
        dx = 0.0f;
        dy = -1.0f;
    }
}


void Spike::Update(const Boss& boss, const Player& player) {
    if (attack >= 0) {
        if (x < 0 || x > 1920 || y < 0 || y > 1080) {
            attack = -1;
            return;
        }
        if (spawncount > 0) {
            spawncount--;
            return; // 대기 중이면 움직이지 않음
        }
        if (attack == 1) {
            if (speed <= 0) {
                cx *= -1;
            }
            else if (speed >= 20) {
                cx *= -1;
            }
            speed += cx;
        }
        bitmapcount = (bitmapcount + 1) % 6;
        x += dx * speed;
        y += dy * speed;
        
    }
}
void Spike::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]) {
    OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[bitmapcount]);
    for (int i = 0; i < 20; i++) {
        TransparentBlt(hdc, x - 15, y - 15, 30, 30, hMemDC, 0, 0, w[bitmapcount], h[bitmapcount], RGB(7, 5, 8));
    }
}