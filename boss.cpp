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

void Boss::Update(const Player& player, int timecount) {
    // 보스 이동 및 공격 패턴을 위한 로직 추가
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
            groggyAngle = angle;
            //mapMove에 따라 정확한 중심 좌표 설정
            if (player.mapMove == 0) {
                centerX = 960;  // 단일 중심
            }
            else if (player.mapMove == 1) {
                // 원 반씩 나뉨: angle 기준 중심 선택
                if (angle < 3.14159f) centerX = 1225;  // 왼쪽 반원
                else centerX = 825;                   // 오른쪽 반원
            }
            else if (player.mapMove == 2) {
                // 사분면 기반
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
            groggytime = 100;
            qcenterX = centerX;
            qcenterY = centerY;
        }
        if (groggytime <= 0) {
            Groggy_ing = true;
            isGroggy = false;
            health = 100;
            return;
        }
        groggytime--;
    }
    else {
        Attack(player,timecount);
    }
}

void Boss::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]) {
    if (!isGroggy) {
        if (Level == 1) {
            // 회전각 (라디안 단위), left turn 이므로 음수
            static float angle = 0.0f;
            angle -= 0.01f; // 회전 속도 조절
            RECT rc = { 0, 0, w[0], h[0] };
            int halfW = w[0] / 2;
            int halfH = h[0] / 2;
            POINT pt[3];

            pt[0].x = (LONG)(halfW + cos(angle) * (-halfW) - sin(angle) * (-halfH));
            pt[0].y = (LONG)(halfH + sin(angle) * (-halfW) + cos(angle) * (-halfH));

            pt[1].x = (LONG)(halfW + cos(angle) * (halfW)-sin(angle) * (-halfH));
            pt[1].y = (LONG)(halfH + sin(angle) * (halfW)+cos(angle) * (-halfH));

            pt[2].x = (LONG)(halfW + cos(angle) * (-halfW) - sin(angle) * (halfH));
            pt[2].y = (LONG)(halfH + sin(angle) * (-halfW) + cos(angle) * (halfH));


            // 1. Yong 로딩
            OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[0]);

            // 2. tempDC에 비트맵 복사 + 투명색 준비
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBmp = CreateCompatibleBitmap(hdc, w[0], h[0]);
            HBITMAP oldTempBmp = (HBITMAP)SelectObject(tempDC, tempBmp);
            FillRect(tempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            TransparentBlt(tempDC, 0, 0, w[0], h[0], hMemDC, 0, 0, w[0], h[0], RGB(255, 255, 255));

            // 3. 회전된 이미지를 위한 회전 DC
            HDC rotDC = CreateCompatibleDC(hdc);
            HBITMAP rotBmp = CreateCompatibleBitmap(hdc, w[0], h[0]);
            HBITMAP oldRotBmp = (HBITMAP)SelectObject(rotDC, rotBmp);
            FillRect(rotDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            // 4. 회전
            PlgBlt(rotDC, pt, tempDC, 0, 0, w[0], h[0], NULL, 0, 0);

            // 5. 최종 출력: 회전된 것만 투명하게 메인 DC로
            TransparentBlt(hdc, x - w[0] / 2, y - h[0] / 2, w[0], h[0], rotDC, 0, 0, w[0], h[0], RGB(255, 255, 255));

            // 6. 정리
            SelectObject(hMemDC, OldBit[2]);
            SelectObject(tempDC, oldTempBmp); DeleteObject(tempBmp); DeleteDC(tempDC);
            SelectObject(rotDC, oldRotBmp); DeleteObject(rotBmp); DeleteDC(rotDC);
        }
        else if (Level == 2 || Level == 3) {
            OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[0]);
            TransparentBlt(hdc, x - w[0] / 2, y - h[0] / 2, w[0], h[0], hMemDC, 0, 0, w[0], h[0], RGB(255, 255, 255));
            SelectObject(hMemDC, OldBit[2]);
        }
    }
    else {
        if (Level == 1) {
            // 회전각 (라디안 단위), left turn 이므로 음수
            static float angle = 0.0f;
            angle -= 0.01f; // 회전 속도 조절
            RECT rc = { 0, 0, w[1], h[1] };
            int halfW = w[1] / 2;
            int halfH = h[1] / 2;
            POINT pt[3];

            pt[0].x = (LONG)(halfW + cos(angle) * (-halfW) - sin(angle) * (-halfH));
            pt[0].y = (LONG)(halfH + sin(angle) * (-halfW) + cos(angle) * (-halfH));

            pt[1].x = (LONG)(halfW + cos(angle) * (halfW)-sin(angle) * (-halfH));
            pt[1].y = (LONG)(halfH + sin(angle) * (halfW)+cos(angle) * (-halfH));

            pt[2].x = (LONG)(halfW + cos(angle) * (-halfW) - sin(angle) * (halfH));
            pt[2].y = (LONG)(halfH + sin(angle) * (-halfW) + cos(angle) * (halfH));


            // 1. Yong 로딩
            OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[1]);

            // 2. tempDC에 비트맵 복사 + 투명색 준비
            HDC tempDC = CreateCompatibleDC(hdc);
            HBITMAP tempBmp = CreateCompatibleBitmap(hdc, w[1], h[1]);
            HBITMAP oldTempBmp = (HBITMAP)SelectObject(tempDC, tempBmp);
            FillRect(tempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            BitBlt(tempDC, 0, 0, w[1], h[1], hMemDC, 0, 0, SRCCOPY);

            // 3. 회전된 이미지를 위한 회전 DC
            HDC rotDC = CreateCompatibleDC(hdc);
            HBITMAP rotBmp = CreateCompatibleBitmap(hdc, w[1], h[1]);
            HBITMAP oldRotBmp = (HBITMAP)SelectObject(rotDC, rotBmp);
            FillRect(rotDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            // 4. 회전
            PlgBlt(rotDC, pt, tempDC, 0, 0, w[1], h[1], NULL, 0, 0);

            // 5. 최종 출력: 회전된 것만 투명하게 메인 DC로
            TransparentBlt(hdc, x - w[1] / 2, y - h[1] / 2, w[1], h[1], rotDC, 0, 0, w[1], h[1], RGB(255, 255, 255));

            // 6. 정리
            SelectObject(hMemDC, OldBit[2]);
            SelectObject(tempDC, oldTempBmp); DeleteObject(tempBmp); DeleteDC(tempDC);
            SelectObject(rotDC, oldRotBmp); DeleteObject(rotBmp); DeleteDC(rotDC);
        }
        else if (Level == 2 || Level == 3) {
            OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[1]);
            TransparentBlt(hdc, x - 50, y - 50, w[1], h[1], hMemDC, 0, 0, w[1], h[1], RGB(255, 255, 255));
            SelectObject(hMemDC, OldBit[2]);
        }
        else {

        }
    }
}
void Boss::Attack(const Player& player,int timecount) {
    int pattern = (timecount % 600);

    if (!lineattack) {
        switch (currentSet) {
        case PatternSet::LV1Combo1:
            if (Level == 3) {
                PlayerY(player);
            }
            else {
                LoopingMove();
            }
            break;
        case PatternSet::LV1Combo2:
            if (Level == 3) {
                FuckingGood(player);
            }
            else {
                LoopingMove();
            }
            break;
        case PatternSet::LV1Combo3:
        case PatternSet::LV1Combo4:
        case PatternSet::Level2_Combo1:
        case PatternSet::OrbitSolo:
        case PatternSet::Level2_OrbitMixed:
            LoopingMove();              // 원형 이동
            Delay = 10;
            break;
        }
        // 선형 떨림 공격 트리거
        if (pattern == 599) {
            lineattack = true;
            lineattackstart = -1;
        }
    }
    else {
        // 선형 떨림 공격 모드
        if (lineattackstart == -1) {
            lineattackstart = timecount;
        }

        int elapsed = timecount - lineattackstart;

        if (elapsed < 180) {
            float frequency = 1.0f + (elapsed / 30.0f);
            float amplitudeX = 20.0f;
            float amplitudeY = 10.0f;
            float centerX = 1920 / 2;
            float centerY = 1080 / 2;

            x = (int)(centerX + sin(elapsed * frequency * 0.1f) * amplitudeX);
            y = (int)(centerY + cos(elapsed * frequency * 0.1f) * amplitudeY);
        }
        else if (elapsed >= 300) {
            // 선형 공격 끝
            lineattack = false;
            lineattackstart = -1;
        }
        else {
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

    // 상반부 (0 < θ < π): 왼쪽 중심 (350, 400)
    // 하반부 (π < θ < 2π): 오른쪽 중심 (450, 400)
    float centerX = 1920 / 2; // 원 궤도 중심 X = 맵 중간
    float centerY = 1080 / 2; // 원 궤도 중심 Y = 맵 중간

    // 각도에 따라 x, y 좌표 계산
    x = cos(angle) * 240 + centerX;
    y = sin(2 * angle) * 80 + centerY;
}
void Boss::CenternMove() {
    float centerX = 1920 / 2; // 원 궤도 중심 X = 맵 중간
    float centerY = 1080 / 2;  // 원 궤도 중심 Y = 맵 중간
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
void Boss::DrawGroggyDanger(HDC hdc, float cx, float cy) {
    if (groggyAngle == 0.0f) return;

    float delta = 0.16f;
    float startAngle = groggyAngle - delta;
    float endAngle = groggyAngle + delta;
    int radius = 350;

    // 바운딩 박스
    int left = cx - radius;
    int top = cy - radius;
    int right = cx + radius;
    int bottom = cy + radius;

    // 시작점, 끝점
    int startX = static_cast<int>(cos(startAngle) * radius + cx);
    int startY = static_cast<int>(sin(startAngle) * radius + cy);
    int endX = static_cast<int>(cos(endAngle) * radius + cx);
    int endY = static_cast<int>(sin(endAngle) * radius + cy);

    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));  // 내부 채우기 없음

    Arc(hdc, left, top, right, bottom, endX, endY, startX, startY);
    
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
    SelectObject(hdc, oldBrush);
}
void Boss::PlayerY(const Player& player) {
    if (A == 0) {
        A = rand() % 2 + 1;
    }
    if (A == 1) {
        x = 100;
    }
    else if (A == 2){
        x = 1820;
    }
    y = player.y;
}
void Boss::FuckingGood(const Player& player) {
    if (Delay > 0) {
        if (B == 0) {
            B = rand() % 4 + 1;
            // 시작 위치 설정
            if (B == 1) {
                x = 400;
                y = 200;
            }
            else if (B == 2) {
                x = 1400;
                y = 200;
            }
            else if (B == 3) {
                x = 400;
                y = 800;
            }
            else if (B == 4) {
                x = 1400;
                y = 800;
            }
        }
        Delay--;
    }
    else {
        if (B != 0) {
            // 방향벡터 설정 (보스 -> 플레이어)
            float dirX = player.x - x;
            float dirY = player.y - y;
            float length = sqrt(dirX * dirX + dirY * dirY);
            if (length != 0) {
                dx = dirX / length;
                dy = dirY / length;
            }
            else {
                dx = 0;
                dy = 0;
            }

            B = 0; // 딱 한 번만 방향 설정
        }

        // 이동
        x += dx * speed * 3;
        y += dy * speed * 3;
        if (x < 0 || x > 1920 || y < 0 || y > 1080) {
            Delay = 10;
        }
    }
}