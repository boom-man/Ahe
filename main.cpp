#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "resource.h"
#include "player.h"
#include "boss.h"
#include "map.h"
#include "mapstyle0.h"
#include "SpikeManager.h"
#include "Item.h"
Item item;
Player player;
Boss boss;
Map map;
MapStyle0 style0;
SpikeManager spikeManager;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GameInit();
void GameUpdate(HWND hwnd);
void GameRender(HDC hdc);
static int j = 9;
static int Count_Clock, Count_Item;
static HBITMAP PlayerBit, CircleMapBit, CircleItemBit, StaminaBit, StaminaItemBit, Circle2Bit, Circle2ItemBit, ItemBIt, Circle3ItemBit, Circle3Bit, MAPBIT;
static BOOL ItemSpawned;


ULONGLONG lastUpdateTime = 0;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdParam, int nCmdShow)
{   
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&WndClass);
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
        0, 0, 1920, 1080, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}
void Stamina_Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit, int x, int y)
{
    HDC hdcmem = CreateCompatibleDC(hdc);
    SelectObject(hdcmem, hbit);

    int staminaLevel = player.StaminaP / 10;

    for (int i = 0; i < staminaLevel; i++)
    {
        TransparentBlt(hdc, x, y - (i * 50), 50, 50, hdcmem, 0, 0, 99, 99, RGB(0, 0, 0));
    }

    DeleteDC(hdcmem);
}
void GameInit() {
    map.Init(xx / 2, yy / 2, 400); // 맵 중심 반지름
    player.Init(xx / 2, yy / 2, 100, 100);  // 플레이어 초기화
    boss.Init(100, 100, 100, spikeManager.Level);    // 보스 초기화
}

void GameUpdate(HWND hwnd, int timecount) {
    player.Update();  // 플레이어 업데이트
    boss.Update(player, timecount);    // 보스 업데이트
    map.Update();     //맵 업데이트        
    spikeManager.Update(boss,player);
    
    if (ItemSpawned && item.CheckCollision(player.GetX(), player.GetY(), player.GetRadius())) {
        ItemType type = item.GetType();

        if (type == MAP_CHANGE1) {
            player.mapMove = 1;
            MAPBIT = Circle2Bit;
        }
        else if (type == MAP_CHANGE2) {
            player.mapMove = 2;
            MAPBIT = Circle3Bit;
        }
        else if (type == MAP_CHANGE3) {
            player.mapMove = 0;
            MAPBIT = CircleMapBit;
        }
        else if (type == STAMINA) {
            player.StaminaP = min(player.StaminaP + 40, 100);
            j = player.StaminaP / 10;
            OutputDebugString(L"[충돌] 스태미나 아이템 획득!\n");
        }

        // 공통 후처리: 아이템 제거 및 상태 초기화
        ItemSpawned = false;
        item.Reset();            // initialized = false, x/y = -100
        ItemBIt = nullptr;       // draw 차단
        Count_Clock = 0;         // 다음 아이템 타이머 초기화
    }
}
void ItemSpawn()
{
    if (ItemSpawned) {
        Count_Clock++;
        return;
    }

    int centerX = 960;
    int centerY = yy / 2;
    int radius = 350;
    float angle = (rand() % 360) * 3.14159f / 180.0f;

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

    // 아이템 생성 조건
    if (Count_Item == 5) {
        Count_Item++;
        item.Init(MAP_CHANGE1, ix, iy);
        ItemBIt = Circle2ItemBit;
        ItemSpawned = true;
        Count_Clock = 0;
    }
    else if (Count_Item == 10) {
        Count_Item++;
        item.Init(MAP_CHANGE2, ix, iy);
        ItemBIt = Circle3ItemBit;
        ItemSpawned = true;
        Count_Clock = 0;
    }
    else if (Count_Item == 15) {
        Count_Item++;
        item.Init(MAP_CHANGE3, ix, iy);
        ItemBIt = CircleItemBit;
        ItemSpawned = true;
        Count_Clock = 0;
        Count_Item = 0;
    }
    else if (Count_Clock == 100) {
        Count_Item++;
        item.Init(STAMINA, ix, iy);
        ItemBIt = StaminaItemBit;
        ItemSpawned = true;
        Count_Clock = 0;
    }
    else {
        Count_Clock++;
    }
}
void GameRender(HDC hdc,int w[],int h[]) {
    TCHAR buffer[64];
    wsprintf(buffer, TEXT("SpikeCount: %d"), spikeManager.spikeCount);
    TextOut(hdc, 10, 10, buffer, lstrlen(buffer)); // 좌표 (10, 10)에 출력
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    PAINTSTRUCT ps;
    HDC hdc,mdc, hMemDC; // 더블버퍼 및 비트맵용
    RECT rt;// 더블 버퍼용
    HBITMAP hBitmap, OldBit[3]; // 비트맵용
    BITMAP bmp; // 비트맵용
    static RECT rectView; // 비트맵 용

    static HBITMAP spikebitmap[4], bossbitmap, rktl[6], boss2bitmap;
    static int BossW,BossH,SpikeW[4], SpikeH[4], PH, PW, BossW2, BossH2;
    static int rktlW[6], rktlH[6];
    HBRUSH hBrush, oldBrush; // 브러쉬

    static int timecount = 0, spawncount = 0;
    static float p_speed;
    switch (msg) {
    case WM_CREATE:
        srand(time(NULL));
        //============================================================================
        hdc = GetDC(hwnd);
        GetClientRect(hwnd, &rectView);
        spikebitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(102));
        spikebitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
        spikebitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
        spikebitmap[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
        for (int i = 0; i < 6; i++) {
            rktl[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(124 + i));
        }
        bossbitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(101));
        boss2bitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(Bossbit2));
        if (!rktl[1]) {
            MessageBox(hwnd, L"보스 비트맵(102)을 불러오지 못했습니다.", L"리소스 오류", MB_ICONERROR);
        }
        player.mapMove = 0;

        PlayerBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

        CircleMapBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        CircleItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));

        Circle2Bit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        Circle2ItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));

        Circle3Bit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
        Circle3ItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));

        StaminaBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        StaminaItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));

        GetObject(bossbitmap, sizeof(BITMAP), &bmp);
        BossW = bmp.bmWidth; BossH = bmp.bmHeight;

        GetObject(boss2bitmap, sizeof(BITMAP), &bmp);
        BossW2 = bmp.bmWidth; BossH2 = bmp.bmHeight;

        for (int i = 0; i < 4; i++) {
            GetObject(spikebitmap[i], sizeof(BITMAP), &bmp);
            SpikeW[i] = bmp.bmWidth; SpikeH[i] = bmp.bmHeight;
        }
        for (int i = 0; i < 6; i++) {
            GetObject(rktl[i], sizeof(BITMAP), &bmp);
            rktlW[i] = bmp.bmWidth; rktlH[i] = bmp.bmHeight;
        }
        GetObject(PlayerBit, sizeof(BITMAP), &bmp);
        PH = bmp.bmHeight; PW = bmp.bmWidth;

        ItemBIt = StaminaItemBit;
        MAPBIT = CircleMapBit;
        //=============================================================================
        SetTimer(hwnd, 1, 60, NULL);
        SetTimer(hwnd, 2, 50, NULL);
        SetTimer(hwnd, 3, 1, NULL); // 범위공격 전용
        map.SetStyle(&style0);
        GameInit();
        //==============================================================================
        p_speed = 1.0f;
        Count_Clock = 0;
        Count_Item = 0;
        item.x = 580;
        item.y = 500;
        ItemSpawned = false;
        break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rt);
        mdc = CreateCompatibleDC(hdc);
        hMemDC = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
        SelectObject(mdc, (HBITMAP)hBitmap);
        hBrush = CreateSolidBrush(RGB(255,255, 255));
        FillRect(mdc, &rt, hBrush);
        oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
        SelectObject(mdc, oldBrush);
        DeleteObject(hBrush);
        //===================================================================
        map.Draw(mdc, hMemDC, MAPBIT, OldBit, 0, 0);
        GameRender(mdc,SpikeW,SpikeH);
        ItemSpawn();
        player.Draw(mdc, PlayerBit, OldBit, PH, PW);
        if (spikeManager.Level == 1) {
            spikeManager.Draw(mdc, hMemDC, rktl, OldBit, rktlW, rktlH);
            boss.Draw(mdc, hMemDC, bossbitmap, OldBit, BossW, BossW);
            if (ItemSpawned && ItemBIt != nullptr && item.IsAlive()) {
                item.Draw(mdc, ItemBIt, hBitmap);
            }
            for (int i = 0; i < 10; i++) {
                spikeManager.yong[i].Draw(mdc, hMemDC, spikebitmap, OldBit, SpikeW, SpikeH);
            }
        }
        else if (spikeManager.Level == 2) {
            spikeManager.Draw(mdc, hMemDC, rktl, OldBit, rktlW, rktlH);
            boss.Draw(mdc, hMemDC, boss2bitmap, OldBit, BossW2, BossH2);
            if (ItemSpawned && ItemBIt != nullptr && item.IsAlive()) {
                item.Draw(mdc, ItemBIt, hBitmap);
            }
            for (int i = 0; i < 10; i++) {
                spikeManager.yong[i].Draw(mdc, hMemDC, spikebitmap, OldBit, SpikeW, SpikeH);
            }
        }
        Stamina_Draw(mdc, StaminaBit, hBitmap, 25, 900);
        //===================================================================
        BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
        DeleteDC(mdc);
        DeleteDC(hMemDC);
        DeleteObject(hBitmap);
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_TIMER:
        if (wp == 1) {
            timecount++;
            GameUpdate(hwnd, timecount);  // 타이머에 의해 게임 업데이트
            InvalidateRect(hwnd, NULL, FALSE);  // 화면 새로 고침
            break;
        }
        if (wp == 2) {
            spawncount++;
            hdc = GetDC(hwnd);
            mdc = CreateCompatibleDC(hdc);
            hMemDC = CreateCompatibleDC(hdc);
            spikeManager.SpawnManager(spikebitmap, SpikeW, SpikeH,
                boss, player, spawncount);
            ReleaseDC(hwnd,hdc);
            DeleteDC(mdc);
            DeleteDC(hMemDC);
        }
        if (wp == 3) {
            for (int i = 0; i < 10; i++) {
                spikeManager.yong[i].Update();
            }
        }
    case WM_KEYDOWN:
        if (wp == VK_RETURN) {
            if (!player.Dash && player.StaminaP > 0) {
                player.Dash = TRUE;
                player.speed *= 5.0f;
                player.SetSpeed(player.speed);
            }
        }
        if (wp == VK_SPACE) {
            player.speed *= -1;
            player.SetSpeed(player.speed);
        }
        if (wp == 81) {
            boss.health -= 20;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case WM_KEYUP:
        if (wp == VK_RETURN) {
            if (player.Dash) {
                player.Dash = FALSE;
                player.speed /= 5.0f;
                player.SetSpeed(player.speed);
            }
        }
        break;
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}