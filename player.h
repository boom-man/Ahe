#pragma once
#include <windows.h>
#define xx 800
#define yy 600
class Player {
public:
    Player();
    void Init(float startX, float startY, int startHealth);
    void SetSpeed(float speed);
    void Update();
    void Draw(HDC hdc);
    void ActivateInvincibility();
    bool Dash;
    float x, y, speed;
private:
    static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // static 멤버 함수 선언

    int health;
    bool invincible;
};