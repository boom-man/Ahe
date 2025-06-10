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
    // ���� ���� ����ȭ
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
    attack = what;         // ���� Ÿ�� (ex: 2 == ȸ��)
    speed = sp;            // ȸ�� �ӵ�
    spawncount = count;    // ���� ��� �ð�

    angle = angleInput;    // �ʱ� ���� (����)
    radius = radiusInput;  // ���� �߽����κ����� �Ÿ�
    during = dr;
    // ��ǥ ���
    x = centerX + cos(angle) * radius;
    y = centerY + sin(angle) * radius;
}

void Spike::Update(const Boss& boss,Player& player,int i) {
    if (attack >= 0) {
        if (spawncount > 0) {
            spawncount--;
            return; // ��� ���̸� �������� ����
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
            // ȸ�� ���� ����: ������ �߽����� ������ũ�� �ð�������� ȸ����

            float centerX = 1920 / 2;  // ������ �߽� x ��ǥ
            float centerY = 1080 /2 ;  // ������ �߽� y ��ǥ

            // ȸ�� �ӵ� ���� (���� Ŭ���� ������ ȸ��)
            float angularSpeed = speed;  // ���� ����

            angle += angularSpeed;  // �ð���� ȸ�� (���� ���� ����)

            // ���� ������ �ʹ� Ŀ���� �ʵ��� 0~2��� ����
            if (angle >= 2 * PI) angle -= 2 * PI;

            // ���ο� ��ġ ��� (������ �������� ��������ŭ ������ ��ġ)
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