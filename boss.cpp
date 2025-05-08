#include "boss.h"
#include <windows.h>
#include <cmath>

Boss::Boss() : x(400), y(300),speed(5.0f), isGroggy(false), health(100) {}

void Boss::Init(float startX, float startY, int startHealth) {
    x = startX;
    y = startY;
    health = startHealth;
}

void Boss::Update(const Player& player) {
    float playerX = player.x;
    float playerY = player.y;
    // ���� �̵� �� ���� ������ ���� ���� �߰�
    if (isGroggy) {
        // �׷α� ���¿��� ������ �ൿ (�������� ����)
        
    }
    else {
        // ������ �غ��ϴ� ����
        static float angle;
        angle += speed * 3.14159f / 180.0f;

        if (angle >= 2 * 3.14159f) {
            angle -= 2 * 3.14159f;
        }

        // ��ݺ� (0 < �� < ��): ���� �߽� (350, 400)
        // �Ϲݺ� (�� < �� < 2��): ������ �߽� (450, 400)
        float centerX = xx / 2; // �� �˵� �߽� X = �� �߰�
        float centerY = yy / 2;  // �� �˵� �߽� Y = �� �߰�

        // ������ ���� x, y ��ǥ ���
        x = cos(angle) * 240 + centerX;
        y = sin(2 * angle) *80 + centerY;
    }
}

void Boss::Attack() {
    // ���� ���� (�������� ���õ� ����)
}

void Boss::Draw(HDC hdc) {
    Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);  // ���� ũ��
}