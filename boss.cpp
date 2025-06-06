#include "Boss.h"
#include <windows.h>
#include <cmath>

Boss::Boss() {
    hBitmap;
    hMemDC;
    x = 1920 / 2;
    y = 1080 / 2;
    speed = 5.0f; 
    isGroggy = false; 
    lineattack = false;
    health = 0;
    bitcount = 0;
    page = 0;
    lineattackstart = -1;
    Groggy_ing = true;
}

void Boss::Init(float startX, float startY, int startHealth, int LV) {
    x = startX;
    y = startY;
    health = startHealth;
    Level = LV;
}

void Boss::Update(const Player& player,int timecount) {
    // ���� �̵� �� ���� ������ ���� ���� �߰�
    if (health <= 0) {
        isGroggy = true;
    }
    
    if (isGroggy) {
        if (Groggy_ing) {
            Groggy_ing = false;
            int centerX = 960;
            int centerY = yy / 2;
            int radius = 350;
            float angle = (rand() % 360) * 3.14159f / 180.0f;

            //mapMove�� ���� ��Ȯ�� �߽� ��ǥ ����
            if (player.mapMove == 0) {
                centerX = 960;  // ���� �߽�
            }
            else if (player.mapMove == 1) {
                // �� �ݾ� ����: angle ���� �߽� ����
                if (angle < 3.14159f) centerX = 1225;  // ���� �ݿ�
                else centerX = 825;                   // ������ �ݿ�
            }
            else if (player.mapMove == 2) {
                // ��и� ���
                float halfPi = 3.14159f / 2;
                if (angle < halfPi) centerX = 1092;
                else if (angle < 3.14159f) centerX = 826;
                else if (angle < 3 * halfPi) centerX = 1619;
                else centerX = 300;
            }
            int ix = static_cast<int>(cos(angle) * radius + centerX);
            int iy = static_cast<int>(sin(angle) * radius + centerY);
            x = ix;
            y = iy;
        }
    }
    else {
        Attack(timecount);
    }
}

void Boss::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong, HBITMAP OldBit[], int w, int h) {
    if (!isGroggy) {
        if (Level == 1) {
            // ȸ���� (���� ����), left turn �̹Ƿ� ����
            static float angle = 0.0f;
            angle -= 0.01f; // ȸ�� �ӵ� ����
            RECT rc = { 0, 0, w, h };
            int halfW = w / 2;
            int halfH = h / 2;
            POINT pt[3];

            pt[0].x = (LONG)(halfW + cos(angle) * (-halfW) - sin(angle) * (-halfH));
            pt[0].y = (LONG)(halfH + sin(angle) * (-halfW) + cos(angle) * (-halfH));

            pt[1].x = (LONG)(halfW + cos(angle) * (halfW)-sin(angle) * (-halfH));
            pt[1].y = (LONG)(halfH + sin(angle) * (halfW)+cos(angle) * (-halfH));

            pt[2].x = (LONG)(halfW + cos(angle) * (-halfW) - sin(angle) * (halfH));
            pt[2].y = (LONG)(halfH + sin(angle) * (-halfW) + cos(angle) * (halfH));


            // 1. Yong �ε�
            OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong);

            // 2. tempDC�� ��Ʈ�� ���� + ����� �غ�
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBmp = CreateCompatibleBitmap(hdc, w, h);
            HBITMAP oldTempBmp = (HBITMAP)SelectObject(tempDC, tempBmp);
            FillRect(tempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            TransparentBlt(tempDC, 0, 0, w, h, hMemDC, 0, 0, w, h, RGB(255, 255, 255));

            // 3. ȸ���� �̹����� ���� ȸ�� DC
            HDC rotDC = CreateCompatibleDC(hdc);
            HBITMAP rotBmp = CreateCompatibleBitmap(hdc, w, h);
            HBITMAP oldRotBmp = (HBITMAP)SelectObject(rotDC, rotBmp);
            FillRect(rotDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            // 4. ȸ��
            PlgBlt(rotDC, pt, tempDC, 0, 0, w, h, NULL, 0, 0);

            // 5. ���� ���: ȸ���� �͸� �����ϰ� ���� DC��
            TransparentBlt(hdc, x - w / 2, y - h / 2, w, h, rotDC, 0, 0, w, h, RGB(255, 255, 255));

            // 6. ����
            SelectObject(hMemDC, OldBit[2]);
            SelectObject(tempDC, oldTempBmp); DeleteObject(tempBmp); DeleteDC(tempDC);
            SelectObject(rotDC, oldRotBmp); DeleteObject(rotBmp); DeleteDC(rotDC);
        }
        else if (Level == 2) {
            OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong);
            TransparentBlt(hdc, x - w / 2, y - h / 2, w, h, hMemDC, 0, 0, w, h, RGB(255, 255, 255));
            SelectObject(hMemDC, OldBit[2]);
        }
    }
    else {
        OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong);
        TransparentBlt(hdc, x -50, y-50, w, h, hMemDC, 0, 0, w, h, RGB(255, 255, 255));   
        SelectObject(hMemDC, OldBit[2]);
    }
}
void Boss::Attack(int timecount) {
    int pattern = (timecount % 100);
    if (!lineattack) {
        if (pattern >= 0 && pattern <= 50) {
            LoopingMove();
        }
        else if (pattern > 70 && pattern <= 98) {
            CenternMove();
        }
        else if (pattern >= 50 && pattern <= 65) {
            FourMove(pattern);
        }
        else if (pattern == 99 && lineattack == false) {
            lineattack = true;
            lineattackstart = -1; // �� ��ȯ �� �ʱ�ȭ
        }
    }
    else {
        if (lineattackstart == -1) {
            lineattackstart = timecount; 
        }
        int elapsed = timecount - lineattackstart;

        if (elapsed < 180) { // 3�� ���� (60fps ����)
            // ���� �ֱ⸦ ���� ª�� ����� ������ ���� ������
            float frequency = 1.0f + (elapsed / 30.0f); // 1.0 ~ 7.0 (���� ����)

            // ���� ���� (�ȼ� ����)
            float amplitudeX = 20.0f;
            float amplitudeY = 10.0f;

            // �߽� ��ǥ
            float centerX = 1920 / 2;
            float centerY = 1080 / 2;

            // �����ķ� ���� ���� (frame �� �������� ���� ���)
            x = (int)(centerX + sin(elapsed * frequency * 0.1f) * amplitudeX);
            y = (int)(centerY + cos(elapsed * frequency * 0.1f) * amplitudeY);
        }
        else if (elapsed >= 300) {
            lineattackstart = -1;

            lineattack = false;
        }
        else {
            // 3�� ������ ����
            x = 1920 / 2;
            y = 1080 / 2;
        }
    }
}
void Boss::LoopingMove() {
    static float angle;
    angle += speed * 3.14159f / 180.0f;

    if (angle >= 2 * 3.14159f) {
        angle -= 2 * 3.14159f;
    }

    // ��ݺ� (0 < �� < ��): ���� �߽� (350, 400)
    // �Ϲݺ� (�� < �� < 2��): ������ �߽� (450, 400)
    float centerX = 1920 / 2; // �� �˵� �߽� X = �� �߰�
    float centerY = 1080 / 2; // �� �˵� �߽� Y = �� �߰�

    // ������ ���� x, y ��ǥ ���
    x = cos(angle) * 240 + centerX;
    y = sin(2 * angle) * 80 + centerY;
}
void Boss::CenternMove() {
    float centerX = 1920 / 2; // �� �˵� �߽� X = �� �߰�
    float centerY = 1080 / 2;  // �� �˵� �߽� Y = �� �߰�
    x = centerX;
    y = centerY;
}
void Boss::FourMove(int pattern) {
    if (pattern % 50 == 0) {
        x = 400;
        y = 200;
    }
    else if (pattern % 50 == 5) {
        x = 1400;
        y = 200;
    }
    else if (pattern % 50 == 10) {
        x = 400;
        y = 800;
    }
    else if (pattern % 50 == 15) {
        x = 1400;
        y = 800;
    }
}