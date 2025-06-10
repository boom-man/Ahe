#include "player.h"
#include "boss.h"
#include <windows.h>
#include <cmath>
#pragma comment (lib, "msimg32.lib")

Player::Player() : x(0), y(0), speed(1.0f), health(100), invincible(false), Dash(false), StaminaP(100) {}
void Player::Init(float startX, float startY, int startHealth, int Stamina) {
    x = startX;
    y = startY;
    health = startHealth;
    StaminaP = Stamina;
    invincible = false;
}
void Player::SetSpeed(float P_speed) {
    speed = P_speed;
}
void Player::Update(Boss& boss) {
    if (delay > 0) {
        delay--;
    }
    // 시간에 따른 각도 계산
    if (Dash) {
        if (StaminaP > 0) {
            StaminaP--;  // 일정 속도로 감소
        }
        else {
            Dash = FALSE;
            speed /= 5.0f;     // Dash 해제 시 원래 속도로 복귀
            SetSpeed(speed);
        }
    }
    // 튕김 처리
    if (bounced) {
        float t = 1.0f - (float)bounceTimer / 10.0f; // 0 ~ 1
        speed = originalspeed * t + (-2.0f * originalspeed) * (1 - t);  // Lerp

        if (--bounceTimer <= 0) {
            speed = originalspeed;  // 정확히 복원
            bounced = false;
        }
    }

    static float angle;
    angle += speed * 3.14159f / 180.0f;
    anglez = angle;
    if (angle >= 2 * 3.14159f) angle -= 2 * 3.14159f;//각도를 360도 범위로 유지
    if (angle < 0) angle += 2 * 3.14159f;//각도를 360도 범위로 유지

    if (mapMove == 0)
    {
        // 상반부 (0 < θ < π): 왼쪽 중심 (350, 400)
        // 하반부 (π < θ < 2π): 오른쪽 중심 (450, 400)
        centerX = xx / 2; // 원 궤도 중심 X = 맵 중간
        centerY = yy / 2;  // 원 궤도 중심 Y = 맵 중간

        // 각도에 따라 x, y 좌표 계산
        x = cos(angle) * 350 + centerX;  // 원 궤도에서 x 좌표 계산
        y = sin(angle) * 350 + centerY;  // 원 궤도에서 y 좌표 계산
    }

    else if (mapMove == 1)
    {

        static int centerState = 0;  // 0: 오른쪽 중심 (825), 1: 왼쪽 중심 (1225)
        static float prevAngle = 0;

        // 중심 전환 조건 (한쪽에서 반대편으로 넘어갈 때만)
        if (prevAngle < 3.14159f && angle >= 3.14159f) {
            centerState = 1;  // 오른쪽 → 왼쪽
        }
        else if (prevAngle >= 3.14159f && angle < 3.14159f) {
            centerState = 0;  // 왼쪽 → 오른쪽
        }

        prevAngle = angle;  // 다음 프레임을 위한 기록

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

        static float angle2 = 0;       // 독립된 각도 변수
        static float prevAngle = 0;
        static int quadrant = 1;
        static bool initialized = false;

        angle2 += speed * pi / 180.0f;

        // 각도 범위 정규화
        if (angle2 >= twoPi) angle2 -= twoPi;
        if (angle2 < 0) angle2 += twoPi;

        // 최초 초기 quadrant 설정
        if (!initialized) {
            if (angle2 < halfPi) quadrant = 1;
            else if (angle2 < pi) quadrant = 2;
            else if (angle2 < 3 * halfPi) quadrant = 3;
            else quadrant = 4;
            initialized = true;
        }

        //방향에 따라 quadrant 전환
        if (speed > 0) {  // 시계방향
            if (prevAngle < halfPi && angle2 >= halfPi && angle2 < pi) quadrant = 2;
            else if (prevAngle < pi && angle2 >= pi && angle2 < 3 * halfPi) quadrant = 3;
            else if (prevAngle < 3 * halfPi && angle2 >= 3 * halfPi && angle2 < twoPi) quadrant = 4;
            else if (prevAngle < twoPi && angle2 < halfPi && angle2 < prevAngle) quadrant = 1;
        }
        else {  // 반시계방향
            if (prevAngle > halfPi && angle2 <= halfPi && angle2 > 0) quadrant = 1;
            else if (prevAngle > pi && angle2 <= pi && angle2 > halfPi) quadrant = 2;
            else if (prevAngle > 3 * halfPi && angle2 <= 3 * halfPi && angle2 > pi) quadrant = 3;
            else if (prevAngle < halfPi && angle2 > 3 * halfPi) quadrant = 4;
        }

        prevAngle = angle2;

        //중심 좌표 설정
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
    
    // 🧠 그로기 상태일 때 각도 체크 → 튕기고 데미지
    if (boss.isGroggy && !bounced) {
        if (angle < boss.groggyAngle + 0.16f && angle > boss.groggyAngle - 0.16) {
            if (Dash) {
                Dash = false;
                speed /= 5.0f;
            }
            originalspeed = speed; // 튕기기 직전의 값=
            speed = -2 * speed;
            bounceTimer = 10;
            bounced = true;
        }
    }
    if (!boss.isGroggy && boss.groggyAngle != 0) {
        if (angle < boss.groggyAngle + 0.16f && angle > boss.groggyAngle - 0.16) {
            OutputDebugString(L"!!! 플레이어 위험구역 진입 !!!\n");
        }
    }
}


void Player::Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit[], int PH, int PW) {
    HDC hdcmem;
    // Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);  // 10은 플레이어 크기
    hdcmem = CreateCompatibleDC(hdc);
    SelectObject(hdcmem, hbit);
    TransparentBlt(hdc, x - 10, y - 10, 20, 20, hdcmem, 0, 0, PW, PH, RGB(0, 6, 22));
    //RGB(238,240, 239)

    DeleteDC(hdcmem);
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

float Player::GetX() const {
    return x;
}

float Player::GetY() const {
    return y;
}

int Player::GetRadius() const {
    return 20;  // 플레이어 반지름, Draw에서 20x20 사용하므로 반지름은 10
}

bool Player::IsInvincible() const {
    return invincible;
}
