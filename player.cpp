#include "player.h"
#include <windows.h>
#include <cmath>

Player::Player() : x(0), y(0), speed(1.0f), health(100), invincible(false),Dash(false) {}

void Player::Init(float startX, float startY, int startHealth) {
    x = startX;
    y = startY;
    health = startHealth;
}
void Player::SetSpeed(float P_speed) {
    speed = P_speed;
}
void Player::Update() {
    // 시간에 따른 각도 계산
    static float angle;
    angle += speed * 3.14159f / 180.0f;

    if (angle >= 2 * 3.14159f) {
        angle -= 2 * 3.14159f;
    }

    // 상반부 (0 < θ < π): 왼쪽 중심 (350, 400)
    // 하반부 (π < θ < 2π): 오른쪽 중심 (450, 400)
    float centerX = xx/2; // 원 궤도 중심 X = 맵 중간
    float centerY = yy/2;  // 원 궤도 중심 Y = 맵 중간

    // 각도에 따라 x, y 좌표 계산
    x = cos(angle) * 240 + centerX;  // 원 궤도에서 x 좌표 계산
    y = sin(angle) * 240 + centerY;  // 원 궤도에서 y 좌표 계산
}

void Player::Draw(HDC hdc) {
    Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);  // 10은 플레이어 크기
}

void Player::ActivateInvincibility() {
    invincible = true;
    // 일정 시간 후 무적 해제 (5초 후)
    SetTimer(NULL, reinterpret_cast<UINT_PTR>(this), 5000, &Player::TimerProc);  // static 멤버 함수 사용
}

// static 멤버 함수 정의
void CALLBACK Player::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    Player* player = reinterpret_cast<Player*>(idEvent);  // idEvent에 전달된 'this' 포인터를 사용
    if (player) {
        player->invincible = false;  // 무적 해제
    }

    KillTimer(hwnd, idEvent);  // 타이머 해제
}