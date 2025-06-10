#pragma once
#include <windows.h>
#define xx 1920
#define yy 1080
class Boss;
class Player {
public:
    Player();
    void Init(float startX, float startY, int startHealth, int StaminaP);
    void SetSpeed(float speed);
    void Update(Boss& boss);
    void Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit[], int PH, int PW);
    void ActivateInvincibility();
    bool Dash, bounced;
    int StaminaP, mapMove, bounceTimer, invincibleTimer;
    float speed, anglez, originalspeed;
    int health;;
    float x, y, centerX,centerY;
    int delay;
    bool invincible;
    float GetX() const;
    float GetY() const;
    int GetRadius() const; // 플레이어 크기에 따라 반지름 반환


    bool IsInvincible() const; // 필요 시 무적 여부 확인
private:
    static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // static 멤버 함수 선언

};