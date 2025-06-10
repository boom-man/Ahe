#include <Windows.h>
#include "Spike.h"
Spike::Spike() : x(0), y(0), attack(-1), speed(5.0f), spawncount(0), cx(-1.0f) {}

void Spike::Init(HBITMAP Yong[], int w[], int h[],
    float startX, float startY, float dirX, float dirY, float sp, int what, int count) {

    x = startX;
    y = startY;
    attack = what;
    speed = sp;
    spawncount = count;
    cx = -1.0f;
    attack4 = false;
    if (what == 5) {
        attack5count = 10;
    }
    // 방향 벡터 정규화
    float length = sqrt(dirX * dirX + dirY * dirY);
    if (length != 0.0f) {
        dx = dirX / length;
        dy = dirY / length;
    }
    else {
        dx = 0.0f;
        dy = -1.0f;
    }
}
void Spike::radiusInit(float centerX, float centerY, int what, float sp, int count, float angleInput, float radiusInput,int dr) {
    attack = what;         // 공격 타입 (ex: 2 == 회전)
    speed = sp;            // 회전 속도
    spawncount = count;    // 스폰 대기 시간

    angle = angleInput;    // 초기 각도 (라디안)
    radius = radiusInput;  // 보스 중심으로부터의 거리
    during = dr;
    // 좌표 계산
    x = centerX + cos(angle) * radius;
    y = centerY + sin(angle) * radius;
}

void Spike::Update(const Boss& boss,Player& player,int i) {
    if (attack >= 0) {
        if (spawncount > 0) {
            spawncount--;
            return; // 대기 중이면 움직이지 않음
        }
        if (!sound) {
            sound = true;
        }
        if (check(player) && !player.invincible){
            if (player.delay <= 0) {
                player.health--;
                player.delay = 20;
            }
        }
        if (attack == 0 || attack == 1 || attack == 4) {
            if (x < 0 || x > 1920 || y < 0 || y > 1080) {
                attack = -1;
                attack4 = false;
                sound = false;
                return;
            }
            if (attack == 1) {
                if (speed <= 0) {
                    cx *= -1;
                }
                else if (speed >= 20) {
                    cx *= -1;
                }
                speed += cx;
            }
            if (!attack4 && (x <= 50 || x >= 1870 || y <= 50 || y >= 1030) && attack == 4) {
                dx *= -3;
                dy *= -3;
                attack4= true;
            }
            x += dx * speed;
            y += dy * speed;
        }
        else if (attack == 2) {
            if (during <= 0) {
                attack = -1;
            }
            // 회전 공격 로직: 보스를 중심으로 스파이크가 시계방향으로 회전함

            float centerX = 1920 / 2;  // 보스의 중심 x 좌표
            float centerY = 1080 /2 ;  // 보스의 중심 y 좌표

            // 회전 속도 조절 (값이 클수록 빠르게 회전)
            float angularSpeed = speed;  // 라디안 단위

            angle += angularSpeed;  // 시계방향 회전 (라디안 각도 증가)

            // 각도 범위가 너무 커지지 않도록 0~2π로 유지
            if (angle >= 2 * PI) angle -= 2 * PI;

            // 새로운 위치 계산 (보스를 기준으로 반지름만큼 떨어진 위치)
            x = centerX + cos(angle) * radius;
            y = centerY + sin(angle) * radius;

            during--;
        }
        bitmapcount = (bitmapcount + 1) % i;
    }
}
void Spike::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]) {
    OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[bitmapcount]);
    TransparentBlt(hdc, x - 15, y - 15, 30, 30, hMemDC, 0, 0, w[bitmapcount], h[bitmapcount], RGB(0, 0, 0));
}

bool Spike::check(Player& player) {
    float dx = x - player.x;
    float dy = y - player.y;
    float distanceSquared = dx * dx + dy * dy;
    float combinedRadius = 15.0f + 10.0f;
    if (distanceSquared <= combinedRadius * combinedRadius) {
        return true;
    }
    return false;
}