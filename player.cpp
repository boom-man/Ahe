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
    // �ð��� ���� ���� ���
    static float angle;
    angle += speed * 3.14159f / 180.0f;

    if (angle >= 2 * 3.14159f) {
        angle -= 2 * 3.14159f;
    }

    // ��ݺ� (0 < �� < ��): ���� �߽� (350, 400)
    // �Ϲݺ� (�� < �� < 2��): ������ �߽� (450, 400)
    float centerX = xx/2; // �� �˵� �߽� X = �� �߰�
    float centerY = yy/2;  // �� �˵� �߽� Y = �� �߰�

    // ������ ���� x, y ��ǥ ���
    x = cos(angle) * 240 + centerX;  // �� �˵����� x ��ǥ ���
    y = sin(angle) * 240 + centerY;  // �� �˵����� y ��ǥ ���
}

void Player::Draw(HDC hdc) {
    Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);  // 10�� �÷��̾� ũ��
}

void Player::ActivateInvincibility() {
    invincible = true;
    // ���� �ð� �� ���� ���� (5�� ��)
    SetTimer(NULL, reinterpret_cast<UINT_PTR>(this), 5000, &Player::TimerProc);  // static ��� �Լ� ���
}

// static ��� �Լ� ����
void CALLBACK Player::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    Player* player = reinterpret_cast<Player*>(idEvent);  // idEvent�� ���޵� 'this' �����͸� ���
    if (player) {
        player->invincible = false;  // ���� ����
    }

    KillTimer(hwnd, idEvent);  // Ÿ�̸� ����
}