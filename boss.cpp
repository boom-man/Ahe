#include "boss.h"
#include <windows.h>

Boss::Boss() : x(0), y(0), isGroggy(false), health(100) {}

void Boss::Init(float startX, float startY, int startHealth) {
    x = startX;
    y = startY;
    health = startHealth;
}

void Boss::Update() {
    // ���� �̵� �� ���� ������ ���� ���� �߰�
    if (isGroggy) {
        // �׷α� ���¿��� ������ �ൿ (�������� ����)
    }
    else {
        // ������ �غ��ϴ� ����
    }
}

void Boss::Attack() {
    // ���� ���� (�������� ���õ� ����)
}

void Boss::Draw(HDC hdc) {
    Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);  // ���� ũ��
}