#include "player.h"
#include <windows.h>
#include <cmath>
#pragma comment (lib, "msimg32.lib")

Player::Player() : x(0), y(0), speed(1.0f), health(100), invincible(false), Dash(false), StaminaP(100) {}
void Player::Init(float startX, float startY, int startHealth, int Stamina) {
    x = startX;
    y = startY;
    health = startHealth;
    StaminaP = Stamina;
}
void Player::SetSpeed(float P_speed) {
    speed = P_speed;
}
void Player::Update() {
    // �ð��� ���� ���� ���
    if (Dash) {
        if (StaminaP > 0) {
            StaminaP--;  // ���� �ӵ��� ����
        }
        else {
            Dash = FALSE;
            speed /= 5.0f;     // Dash ���� �� ���� �ӵ��� ����
            SetSpeed(speed);
        }
    }
    static float angle;
    angle += speed * 3.14159f / 180.0f;

    if (angle >= 2 * 3.14159f) angle -= 2 * 3.14159f;//������ 360�� ������ ����
    if (angle < 0) angle += 2 * 3.14159f;//������ 360�� ������ ����

    if (mapMove == 0)
    {
        // ��ݺ� (0 < �� < ��): ���� �߽� (350, 400)
        // �Ϲݺ� (�� < �� < 2��): ������ �߽� (450, 400)
        centerX = xx / 2; // �� �˵� �߽� X = �� �߰�
        centerY = yy / 2;  // �� �˵� �߽� Y = �� �߰�

        // ������ ���� x, y ��ǥ ���
        x = cos(angle) * 350 + centerX;  // �� �˵����� x ��ǥ ���
        y = sin(angle) * 350 + centerY;  // �� �˵����� y ��ǥ ���
    }

    else if (mapMove == 1)
    {

        static int centerState = 0;  // 0: ������ �߽� (825), 1: ���� �߽� (1225)
        static float prevAngle = 0;

        // �߽� ��ȯ ���� (���ʿ��� �ݴ������� �Ѿ ����)
        if (prevAngle < 3.14159f && angle >= 3.14159f) {
            centerState = 1;  // ������ �� ����
        }
        else if (prevAngle >= 3.14159f && angle < 3.14159f) {
            centerState = 0;  // ���� �� ������
        }

        prevAngle = angle;  // ���� �������� ���� ���

        centerX = (centerState == 0) ? 1225 : 825;
        centerY = yy / 2;
        float radius = 350;

        x = cos(angle) * radius + centerX;
        y = sin(angle) * radius + centerY;

    }

    else if (mapMove == 2)
    {
        float pi = 3.14159f;
        float halfPi = pi / 2;
        float twoPi = 2 * pi;

        static float angle2 = 0;       // ������ ���� ����
        static float prevAngle = 0;
        static int quadrant = 1;
        static bool initialized = false;

        angle2 += speed * pi / 180.0f;

        // ���� ���� ����ȭ
        if (angle2 >= twoPi) angle2 -= twoPi;
        if (angle2 < 0) angle2 += twoPi;

        // ���� �ʱ� quadrant ����
        if (!initialized) {
            if (angle2 < halfPi) quadrant = 1;
            else if (angle2 < pi) quadrant = 2;
            else if (angle2 < 3 * halfPi) quadrant = 3;
            else quadrant = 4;
            initialized = true;
        }

        //���⿡ ���� quadrant ��ȯ
        if (speed > 0) {  // �ð����
            if (prevAngle < halfPi && angle2 >= halfPi && angle2 < pi) quadrant = 2;
            else if (prevAngle < pi && angle2 >= pi && angle2 < 3 * halfPi) quadrant = 3;
            else if (prevAngle < 3 * halfPi && angle2 >= 3 * halfPi && angle2 < twoPi) quadrant = 4;
            else if (prevAngle < twoPi && angle2 < halfPi && angle2 < prevAngle) quadrant = 1;
        }
        else {  // �ݽð����
            if (prevAngle > halfPi && angle2 <= halfPi && angle2 > 0) quadrant = 1;
            else if (prevAngle > pi && angle2 <= pi && angle2 > halfPi) quadrant = 2;
            else if (prevAngle > 3 * halfPi && angle2 <= 3 * halfPi && angle2 > pi) quadrant = 3;
            else if (prevAngle < halfPi && angle2 > 3 * halfPi) quadrant = 4;
        }

        prevAngle = angle2;

        //�߽� ��ǥ ����
        switch (quadrant) {
        case 1: centerX = 1092; break;
        case 2: centerX = 826; break;
        case 3: centerX = 1619; break;
        case 4: centerX = 300; break;
        }

        centerY = yy / 2;
        float radius = 350;

        x = cos(angle2) * radius + centerX;
        y = sin(angle2) * radius + centerY;
    }


}


void Player::Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit[], int PH, int PW) {
    HDC hdcmem;
    // Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);  // 10�� �÷��̾� ũ��
    hdcmem = CreateCompatibleDC(hdc);
    SelectObject(hdcmem, hbit);
    TransparentBlt(hdc, x - 10, y - 10, 20, 20, hdcmem, 0, 0, PW, PH, RGB(0, 6, 22));
    //RGB(238,240, 239)

    DeleteDC(hdcmem);

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

float Player::GetX() const {
    return x;
}

float Player::GetY() const {
    return y;
}

int Player::GetRadius() const {
    return 20;  // �÷��̾� ������, Draw���� 20x20 ����ϹǷ� �������� 10
}

bool Player::IsInvincible() const {
    return invincible;
}

