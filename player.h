#pragma once
#include <windows.h>
#define xx 1920
#define yy 1080
class Player {
public:
    Player();
    void Init(float startX, float startY, int startHealth, int StaminaP);
    void SetSpeed(float speed);
    void Update();
    void Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit[], int PH, int PW);
    void ActivateInvincibility();
    bool Dash;
    int StaminaP, mapMove;
    float speed, health;;
    float x, y, centerX,centerY;
    float GetX() const;
    float GetY() const;
    int GetRadius() const; // 플레이어 크기에 따라 반지름 반환
    bool IsInvincible() const; // 필요 시 무적 여부 확인
private:
    static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // static 멤버 함수 선언

    bool invincible;
};