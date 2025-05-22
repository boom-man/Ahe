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
    // 보스 이동 및 공격 패턴을 위한 로직 추가
    if (isGroggy) {
        // 그로기 상태에서 보스의 행동 (움직이지 않음)
        
    }
    else {
        Attack(timecount);
    }
}

void Boss::Draw(HDC hdc) {
    Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);  // 보스 크기
}

void Boss::Attack(int timecount) {
    int pattern = (timecount / 100) % 10;

    if (pattern == 0) {
        // 공격을 준비하는 상태
        static float angle;
        angle += speed * 3.14159f / 180.0f;

        if (angle >= 2 * 3.14159f) {
            angle -= 2 * 3.14159f;
        }

        // 상반부 (0 < θ < π): 왼쪽 중심 (350, 400)
        // 하반부 (π < θ < 2π): 오른쪽 중심 (450, 400)
        float centerX = 400; // 원 궤도 중심 X = 맵 중간
        float centerY = 300; // 원 궤도 중심 Y = 맵 중간

        // 각도에 따라 x, y 좌표 계산
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
    float centerX = 400; // 원 궤도 중심 X = 맵 중간
    float centerY = 300;  // 원 궤도 중심 Y = 맵 중간
    x = centerX;
    y = centerY;
}
void Boss::XShapedSpikes() {
    float centerX = 400; // 원 궤도 중심 X = 맵 중간
    float centerY = 300;  // 원 궤도 중심 Y = 맵 중간
    x = centerX;
    y = centerY;
}
void Boss::SinglePointSpikes() {
    float centerX = 400; // 원 궤도 중심 X = 맵 중간
    float centerY = 300;  // 원 궤도 중심 Y = 맵 중간
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