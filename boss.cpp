#include "boss.h"
#include <windows.h>

Boss::Boss() : x(0), y(0), isGroggy(false), health(100) {}

void Boss::Init(float startX, float startY, int startHealth) {
    x = startX;
    y = startY;
    health = startHealth;
}

void Boss::Update() {
    // 보스 이동 및 공격 패턴을 위한 로직 추가
    if (isGroggy) {
        // 그로기 상태에서 보스의 행동 (움직이지 않음)
    }
    else {
        // 공격을 준비하는 상태
    }
}

void Boss::Attack() {
    // 공격 패턴 (랜덤으로 선택된 공격)
}

void Boss::Draw(HDC hdc) {
    Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);  // 보스 크기
}