#include "Boss.h"
#include <windows.h>
#include <cmath>

Boss::Boss() {
    hBitmap;
    hMemDC;
    x = 400;
    y = 300;
    speed = 5.0f; 
    isGroggy = false; 
    health = 100;
    bitcount = 0;
}

void Boss::Init(float startX, float startY, int startHealth) {
    x = startX;
    y = startY;
    health = startHealth;
}

void Boss::Update(const Player& player,int timecount) {
    // ���� �̵� �� ���� ������ ���� ���� �߰�
    if (isGroggy) {
        // �׷α� ���¿��� ������ �ൿ (�������� ����)
        
    }
    else {
        Attack(timecount);
    }
}

void Boss::Draw(HDC hdc) {
    Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);  // ���� ũ��
}

void Boss::Attack(int timecount) {
    int pattern = (timecount / 100) % 10;

    if (pattern == 0) {
        // ������ �غ��ϴ� ����
        static float angle;
        angle += speed * 3.14159f / 180.0f;

        if (angle >= 2 * 3.14159f) {
            angle -= 2 * 3.14159f;
        }

        // ��ݺ� (0 < �� < ��): ���� �߽� (350, 400)
        // �Ϲݺ� (�� < �� < 2��): ������ �߽� (450, 400)
        float centerX = 400; // �� �˵� �߽� X = �� �߰�
        float centerY = 300; // �� �˵� �߽� Y = �� �߰�

        // ������ ���� x, y ��ǥ ���
        x = cos(angle) * 240 + centerX;
        y = sin(2 * angle) * 80 + centerY;
    }
    else if (pattern == 1) {
        CrossSpikes();
    }
    else if (pattern == 2) {
        XShapedSpikes();
    }
    else if (pattern == 3) {
        SinglePointSpikes();
    }
    else if (pattern == 4) {
        OrbitingAttack();
    }
    else if (pattern == 5) {
        WideAreaSpikes();
    }
    else if (pattern == 6) {
        PeripheralSpikes();
    }
    else if (pattern == 7) {
        RandomOrbitSpikes();
    }
    else if (pattern == 8) {
        OrbitTimeAttack();
    }
    else if (pattern == 9) {
        TrackingOrbAttack();
    }
}
void Boss::CrossSpikes() {
    float centerX = 400; // �� �˵� �߽� X = �� �߰�
    float centerY = 300;  // �� �˵� �߽� Y = �� �߰�
    x = centerX;
    y = centerY;
}
void Boss::XShapedSpikes() {
    float centerX = 400; // �� �˵� �߽� X = �� �߰�
    float centerY = 300;  // �� �˵� �߽� Y = �� �߰�
    x = centerX;
    y = centerY;
}
void Boss::SinglePointSpikes() {
    float centerX = 400; // �� �˵� �߽� X = �� �߰�
    float centerY = 300;  // �� �˵� �߽� Y = �� �߰�
    x = centerX;
    y = centerY;
}
void Boss::OrbitingAttack() {

}
void Boss::WideAreaSpikes() {

}
void Boss::PeripheralSpikes() {

}
void Boss::RandomOrbitSpikes() {

}
void Boss::OrbitTimeAttack() {

}
void Boss::TrackingOrbAttack() {

}