#include <Windows.h>
#include "Spike.h"
Spike::Spike() : x(0), y(0), attack(-1), speed(5.0f), spawncount(0) {}

void Spike::Init(float startX, float startY, float dirX, float dirY, float sp, int what, int count) {
    x = startX;
    y = startY;

    // ���� ���͸� ���� ���ͷ� ����ȭ
    float length = sqrt(dirX * dirX + dirY * dirY);
    if (length != 0.0f) {
        dx = dirX / length;
        dy = dirY / length;
    }
    else {
        dx = 0.0f;
        dy = -1.0f; // �⺻��: ����
    }

    speed = sp; // �⺻ �ӵ�

    attack = what;

    spawncount = count;
}

void Spike::Update(const Boss& boss, const Player& player) {
    if (attack >= 0) {
        if (x < 0 || x > 800 || y < 0 || y > 600) {
            attack = -1;
            return;
        }
        if (spawncount > 0) {
            spawncount--;
            return; // ��� ���̸� �������� ����
        }
        x += dx * speed;
        y += dy * speed;
        
    }
}
void Spike::Draw(HDC hdc) {
    if (attack >= 0) {
        if (spawncount == 0) {
            Rectangle(hdc, x - 10, y - 10, x + 10, y + 10);
        }
        if (attack == 1 && spawncount > 0) {
            // ������ ��� ����
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBM = CreateCompatibleBitmap(hdc, 20, 20); // ������ũ ũ�⸸ŭ
            HBITMAP oldBM = (HBITMAP)SelectObject(memDC, memBM);

            // ���� ��� �׸���
            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            RECT rc = { 0, 0, 20, 20 };
            FillRect(memDC, &rc, redBrush);

            // AlphaBlend�� ������ ���
            BLENDFUNCTION blend = {};
            blend.BlendOp = AC_SRC_OVER;
            blend.SourceConstantAlpha = 255 - spawncount * 10;
            blend.AlphaFormat = 0;

            AlphaBlend(hdc, (int)(x - 10), 0, 20, 900, memDC, 0, 0, 20, 20, blend);

            // ����
            DeleteObject(redBrush);
            SelectObject(memDC, oldBM);
            DeleteObject(memBM);
            DeleteDC(memDC);
        }
    }
}