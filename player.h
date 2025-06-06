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
    int GetRadius() const; // �÷��̾� ũ�⿡ ���� ������ ��ȯ
    bool IsInvincible() const; // �ʿ� �� ���� ���� Ȯ��
private:
    static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // static ��� �Լ� ����

    bool invincible;
};