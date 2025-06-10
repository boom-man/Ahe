#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h> 
#include "resource.h"
#include "player.h"
#include "boss.h"
#include "map.h"
#include "mapstyle0.h"
#include "SpikeManager.h"
#include "Item.h"
#include "Menu.h"
#include <gdiplus.h>
#define SetLevelTracked(lv) spikeManager.SetLevel_Debug(lv, __FILE__, __LINE__, __func__)
using namespace Gdiplus;

#define easy 5000
#define normal 6000
#define hard 7000

ULONG_PTR gdiplusToken;

void InitGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void ShutdownGDIPlus() {
    GdiplusShutdown(gdiplusToken);
}
Item item;
Player player;
Boss boss;
Map map;
MapStyle0 style0;
SpikeManager spikeManager;
Menu menu;
int lv = spikeManager.GetLevel();//1212121212
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GameInit();
void GameUpdate(HWND hwnd);
void GameRender(HDC hdc,int timecount);
static int j = 9;
static bool game_start = false;

static int game_start_count = 3; static int digits[5];
static int Count_Clock, Count_Item, GameDifficulty, HeartAmount, AttackChargeAmmount, AttackTimeCount, Score_Count;
static HBITMAP PlayerBit, CircleMapBit, CircleItemBit, StaminaBit,
StaminaItemBit, AttackItemBit, AttackChargeBit, AttackFrameBit,
Circle2Bit, Circle2ItemBit, ItemBIt, Circle3ItemBit, Circle3Bit,
MAPBIT, HeartBit, NumberBit[10]; 
static int volume = 500;
static BOOL ItemSpawned, GameStart, selected_done, Attack, GameEnd;


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

void Heart_Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit, int x, int y)
{
    HDC hdcmem = CreateCompatibleDC(hdc);
    SelectObject(hdcmem, hbit);


    for (int i = 0; i < player.health; i++)
    {
        TransparentBlt(hdc, x, y - (i * 50), 50, 50, hdcmem, 0, 0, 99, 99, RGB(0, 0, 0));
    }

    DeleteDC(hdcmem);
}
void Attack_Charge_Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit, int x, int y)
{
    HDC hdcmem = CreateCompatibleDC(hdc);
    SelectObject(hdcmem, AttackFrameBit);
    TransparentBlt(hdc, 0, 0, 250, 100, hdcmem, 0, 0, 1000, 400, RGB(0, 0, 0));

    SelectObject(hdcmem, hbit);
    for (int i = 0; i < AttackChargeAmmount; i++)
    {
        TransparentBlt(hdc, x + (i * 50), y, 50, 50, hdcmem, 0, 0, 99, 99, RGB(0, 0, 0));
    }

    DeleteDC(hdcmem);
}

void Attack_Draw(HDC hdc, HBITMAP hbit, HBITMAP OldBit, int x, int y)
{
    HDC hdcmem = CreateCompatibleDC(hdc);

    SelectObject(hdcmem, hbit);

    TransparentBlt(hdc, boss.x - 75, boss.y - 75, 150, 150, hdcmem, 0, 0, 99, 99, RGB(0, 0, 0));

    DeleteDC(hdcmem);
}
void GameInit() {
    map.Init(xx / 2, yy / 2, 400); // 맵 중심 반지름
    player.Init(xx / 2, yy / 2, player.health, 100);  // 플레이어 초기화
}
void ResetGame(int* spawncount, int* timecount) {
    // 플레이어 초기화
    player.Init(xx / 2, yy / 2, 10, 100);  // 체력을 기본값(예: 10)으로 설정
    player.mapMove = 0;
    player.speed = 1.0f;
    player.StaminaP = 100;
    player.Dash = FALSE;

    // 보스 초기화
    boss.Init(xx / 2, yy / 2, 100, spikeManager.GetLevel());

    // 아이템 초기화
    item.Reset();
    ItemSpawned = false;
    Count_Clock = 0;
    Count_Item = 0;

    // 스파이크 매니저 초기화
    spikeManager.Reset();  // SpikeManager 클래스에 Reset() 메서드 구현 필요

    // 게임 상태 변수 리셋
    (*timecount) = 0;
    (*spawncount) = 0;
    game_start = false;
    game_start_count = 3;
    Attack = false;
    AttackChargeAmmount = 0;
    boss.groggyAngle == 0.0f;

    MAPBIT = CircleMapBit;
}

void GameUpdate(HWND hwnd, int* timecount) {
    boss.Update(player, timecount);    // 보스 업데이트
    map.Update();     //맵 업데이트        
    spikeManager.Update(boss,player);
    
    if (ItemSpawned && item.CheckCollision(player.GetX(), player.GetY(), player.GetRadius())) {
        ItemType type = item.GetType();

        if (type == MAP_CHANGE1) {
            player.mapMove = 1;
            boss.lineattack = false;
            boss.groggyAngle = 0.0f;
            MAPBIT = Circle2Bit;
        }
        else if (type == MAP_CHANGE2) {
            player.mapMove = 2;
            boss.lineattack = false;
            boss.groggyAngle = 0.0f;
            MAPBIT = Circle3Bit;
        }
        else if (type == MAP_CHANGE3) {
            player.mapMove = 0;
            boss.lineattack = false;
            boss.groggyAngle = 0.0f;
            MAPBIT = CircleMapBit;
        }
        else if (type == STAMINA) {
            player.StaminaP = min(player.StaminaP + 40, 100);
            j = player.StaminaP / 10;
            OutputDebugString(L"[충돌] 스태미나 아이템 획득!\n");
        }
        else if (type == ATTACK) {
            if (AttackChargeAmmount == 3)
                AttackChargeAmmount = 3;
            else
                AttackChargeAmmount++;
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
        if (Count_Item % 2 == 0) {
            item.Init(STAMINA, ix, iy);
            ItemBIt = StaminaItemBit;
        }
        else
        {
            item.Init(ATTACK, ix, iy);
            ItemBIt = AttackItemBit;
        }
        ItemSpawned = true;
        Count_Clock = 0;
    }
    else {
        Count_Clock++;
    }
}

void ScoreDraw(HDC hdc)
{

    HDC hdcmem = CreateCompatibleDC(hdc);

    SelectObject(hdcmem, NumberBit[digits[0]]);
    TransparentBlt(hdc, 950, 375, 150, 300, hdcmem, 0, 0, 150, 300, RGB(0, 0, 0));

    SelectObject(hdcmem, NumberBit[digits[1]]);
    TransparentBlt(hdc, 1100, 375, 150, 300, hdcmem, 0, 0, 150, 300, RGB(0, 0, 0));

    SelectObject(hdcmem, NumberBit[digits[2]]);
    TransparentBlt(hdc, 1250, 375, 150, 300, hdcmem, 0, 0, 150, 300, RGB(0, 0, 0));

    SelectObject(hdcmem, NumberBit[digits[3]]);
    TransparentBlt(hdc, 1400, 375, 150, 300, hdcmem, 0, 0, 150, 300, RGB(0, 0, 0));

    SelectObject(hdcmem, NumberBit[digits[4]]);
    TransparentBlt(hdc, 1550, 375, 150, 300, hdcmem, 0, 0, 150, 300, RGB(0, 0, 0));

    DeleteDC(hdcmem);
}

void ScoreCal(int x)
{
    digits[0] = x / 10000;          // 만의 자리
    digits[1] = (x % 10000) / 1000; // 천의 자리
    digits[2] = (x % 1000) / 100;   // 백의 자리
    digits[3] = (x % 100) / 10;     // 십의 자리
    digits[4] = x % 10;               //일의 자리
}
void GameRender(HDC hdc,int spawncount) {
    TCHAR buffer[64];
    _stprintf(buffer, TEXT("%d %d"),spawncount,boss.go);
    TextOut(hdc, 10, 10, buffer, lstrlen(buffer)); // 좌표 (10, 10)에 출력
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    PAINTSTRUCT ps;
    HDC hdc,mdc, hMemDC; // 더블버퍼 및 비트맵용
    RECT rt;// 더블 버퍼용
    HBITMAP hBitmap, OldBit[3]; // 비트맵용
    BITMAP bmp; // 비트맵용
    static RECT rectView; // 비트맵 용

    static HBITMAP spikebitmap[4], bossbitmap[2], rktl[6], boss2bitmap[2], LV1_rktl[3], boss13[2],LV3_rktl[1];
    static int BossW[2], BossH[2], SpikeW[4], SpikeH[4], PH, PW, BossW2[2], BossH2[2],bossW13[2],bossH13[2];
    static int rktlW[6], rktlH[6], LV1_rktlW[3], LV1_rktlH[3],LV3_rktlW[1],LV3_rktlH[1];
    HBRUSH hBrush, oldBrush; // 브러쉬
    static float p_speed;

    static int timecount = 0, spawncount = 0;

    static bool sound1 = false;
    static int sound1count = 0;
    switch (msg) {
    case WM_CREATE:
        mciSendString(TEXT("open \"res//AVOID N SURVIVE 음악.wav\" type mpegvideo alias bgm"), NULL, 0, NULL);
        mciSendString(TEXT("play bgm repeat"), NULL, 0, NULL);
        srand(time(NULL));
        SetTimer(hwnd, 77, 100, NULL); // 소리 제어
        //============================================================================
        hdc = GetDC(hwnd);
        GetClientRect(hwnd, &rectView);
        boss13[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
        boss13[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));


        spikebitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(102));
        spikebitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
        spikebitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
        spikebitmap[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
        for (int i = 0; i < 6; i++) {
            rktl[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(124 + i));
        }
        for (int i = 0; i < 3; i++) {
            LV1_rktl[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(131 + i));
        }
        HeartBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
        bossbitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(101));
        bossbitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(135));
        boss2bitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(Bossbit2));
        boss2bitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
        
        player.mapMove = 0;

        //START_STARTBIT, START_EXITBIT, EASYBIT, NORMALBIT, HARDBIT
        menu.START_MENUBIT = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
        menu.START_STARTBIT = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
        menu.START_EXITBIT = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
        menu.EASYBIT = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
        menu.NORMALBIT = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
        menu.HARDBIT = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
        menu.MenuButtonBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(Menu_Button));
        menu.EndBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(End_));

        PlayerBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

        CircleMapBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        CircleItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));

        Circle2Bit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        Circle2ItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));

        Circle3Bit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
        Circle3ItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));

        StaminaBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        StaminaItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        
        for (int i = 0; i < 10; i++) NumberBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(Number0 + i));
        AttackItemBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(AttackItem));
        AttackChargeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(AttackCharge));
        AttackFrameBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(AttackFrame));

        GetObject(boss13[0], sizeof(BITMAP), &bmp);
        bossW13[0] = bmp.bmWidth; bossH13[0] = bmp.bmHeight;

        GetObject(boss13[1], sizeof(BITMAP), &bmp);
        bossW13[1] = bmp.bmWidth; bossH13[1] = bmp.bmHeight;

        GetObject(bossbitmap[0], sizeof(BITMAP), &bmp);
        BossW[0] = bmp.bmWidth; BossH[0] = bmp.bmHeight;

        GetObject(bossbitmap[1], sizeof(BITMAP), &bmp);
        BossW[1] = bmp.bmWidth; BossH[1] = bmp.bmHeight;

        GetObject(boss2bitmap[0], sizeof(BITMAP), &bmp);
        BossW2[0] = bmp.bmWidth; BossH2[0] = bmp.bmHeight;

        GetObject(boss2bitmap[1], sizeof(BITMAP), &bmp);
        BossW2[1] = bmp.bmWidth; BossH2[1] = bmp.bmHeight;

        for (int i = 0; i < 4; i++) {
            GetObject(spikebitmap[i], sizeof(BITMAP), &bmp);
            SpikeW[i] = bmp.bmWidth; SpikeH[i] = bmp.bmHeight;
        }
        for (int i = 0; i < 6; i++) {
            GetObject(rktl[i], sizeof(BITMAP), &bmp);
            rktlW[i] = bmp.bmWidth; rktlH[i] = bmp.bmHeight;
        }
        for (int i = 0; i < 3; i++) {
            GetObject(LV1_rktl[i], sizeof(BITMAP), &bmp);
            LV1_rktlW[i] = bmp.bmWidth; LV1_rktlH[i] = bmp.bmHeight;
        }
        GetObject(PlayerBit, sizeof(BITMAP), &bmp);
        PH = bmp.bmHeight; PW = bmp.bmWidth;

        ItemBIt = StaminaItemBit;
        MAPBIT = CircleMapBit;
        //=============================================================================
        SetTimer(hwnd, 1, 50, NULL);
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
        GameStart = false;
        menu.Select_Difficulty = false;
        selected_done = false;
        HeartAmount = 0;
        Attack = false;
        break;
    case WM_PAINT:
    {
        InitGDIPlus();
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
        player.Draw(mdc, PlayerBit, OldBit, PH, PW);
        if (!GameStart && !GameEnd)
        {
            menu.MenuDraw(mdc);
        }
        else if (GameStart){
            if (player.health <= 0) {
                Score_Count += player.bossatackpoint;
                ScoreCal(Score_Count);
                GameStart = false;
                GameEnd = true;
                boss.groggyAngle = 0;
                boss.isGroggy = false;
                boss.lineattack = false;
                spawncount = 0;
                timecount = 0;
                InvalidateRect(hwnd, NULL, FALSE);  // 화면 갱신
                break;
            }

            ItemSpawn();
            Heart_Draw(mdc, HeartBit, hBitmap, 1825, 900);
            Attack_Charge_Draw(mdc, AttackChargeBit, hBitmap, 50, 25);
            if (spikeManager.GetLevel() == 1) {
                if (game_start) {
                    spikeManager.Draw(mdc, hMemDC, LV1_rktl, OldBit, LV1_rktlW, LV1_rktlH);
                }
                boss.Draw(mdc, hMemDC, bossbitmap, OldBit, BossW, BossW);
                if (ItemSpawned && ItemBIt != nullptr && item.IsAlive()) {
                    item.Draw(mdc, ItemBIt, hBitmap);
                }
                for (int i = 0; i < 10; i++) {
                    spikeManager.yong[i].Draw(mdc, hMemDC, spikebitmap, OldBit, SpikeW, SpikeH);
                }
            }
            else  if (spikeManager.GetLevel() == 2) {
                if (game_start) {
                    spikeManager.Draw(mdc, hMemDC, rktl, OldBit, rktlW, rktlH);
                }
                boss.Draw(mdc, hMemDC, boss2bitmap, OldBit, BossW2, BossH2);
                if (ItemSpawned && ItemBIt != nullptr && item.IsAlive()) {
                    item.Draw(mdc, ItemBIt, hBitmap);
                }
                for (int i = 0; i < 30; i++) {
                    spikeManager.yong[i].Draw(mdc, hMemDC, spikebitmap, OldBit, SpikeW, SpikeH);
                }
            }
            else {
                if (game_start) {
                    spikeManager.Draw(mdc, hMemDC, rktl, OldBit, rktlW, rktlH);
                }
                boss.Draw(mdc, hMemDC, boss13, OldBit, bossW13, bossH13);
                if (ItemSpawned && ItemBIt != nullptr && item.IsAlive()) {
                    item.Draw(mdc, ItemBIt, hBitmap);
                }
                for (int i = 0; i < 30; i++) {
                    spikeManager.yong[i].Draw(mdc, hMemDC, spikebitmap, OldBit, SpikeW, SpikeH);
                }
            }
            Stamina_Draw(mdc, StaminaBit, hBitmap, 25, 900);
            if (Attack) Attack_Draw(mdc, AttackChargeBit, hBitmap, 0, 0);
            Score_Count++;
        }
        else if (GameEnd) {
            menu.MenuEndDraw(mdc);
            ScoreDraw(mdc);
        }
        if (!boss.isGroggy && boss.groggyAngle != 0.0f) {
            boss.DrawGroggyDanger(mdc, boss.qcenterX, boss.qcenterY);
        }
        //GameRender(mdc, timecount);
        //===================================================================
        BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
        DeleteDC(mdc);
        DeleteDC(hMemDC);
        DeleteObject(hBitmap);
        EndPaint(hwnd, &ps);
        ShutdownGDIPlus();
    }
    break;

    case WM_TIMER:
        if (GameStart) {
            if (wp == 69 && !game_start) {
                game_start_count--;
            }
            if (game_start_count <= 0) {
                game_start = true;
            }
            if (wp == 1) {
                if (game_start) {
                    timecount++;
                    GameUpdate(hwnd, &timecount);  // 타이머에 의해 게임 업데이트
                }
                player.Update(boss);  // 플레이어 업데이트
                if (Attack) {
                    if (AttackTimeCount == 15) {
                        Attack = false;
                        AttackTimeCount = 0;
                    }
                    AttackTimeCount++;

                }
            }
            if (wp == 2) {
                if (game_start) {
                    spawncount++;
                }
                hdc = GetDC(hwnd);
                mdc = CreateCompatibleDC(hdc);
                hMemDC = CreateCompatibleDC(hdc);
                spikeManager.SpawnManager(spikebitmap, SpikeW, SpikeH, hdc,
                    boss, player, spawncount);
                ReleaseDC(hwnd, hdc);
                DeleteDC(mdc);
                DeleteDC(hMemDC);
            }
            if (wp == 3) {
                for (int i = 0; i < 30; i++) {
                    spikeManager.yong[i].Update(player, SpikeW, SpikeH);
                }
                if (spikeManager.GetLevel() == 2) {
                    int a;
                }
                InvalidateRect(hwnd, NULL, FALSE);  // 화면 새로 고침
            }
        }
        else {
            if (wp == 1) {
                player.StaminaP = 100;
                player.Update(boss);
                InvalidateRect(hwnd, NULL, FALSE);  // 화면 새로 고침
            }
        }
        if (wp == 77) {
            if (sound1) {
                sound1count--;
                if (sound1count == 0) {
                    mciSendString(TEXT("close sfx"), NULL, 0, NULL);
                    sound1 = false;
                }
            }
            if (player.attacksound) {
                player.soundcount--;
                if (player.soundcount == 0) {
                    mciSendString(TEXT("close attackboss"), NULL, 0, NULL);
                    player.attacksound = false;
                }
            }
        }
        if (boss.isGroggy) spawncount = 0;
        break;
    case WM_KEYDOWN:
        if (!player.bounced) {
            if (wp == VK_RETURN) {
                if (!player.Dash && player.StaminaP > 0) {
                    player.Dash = TRUE;
                    player.speed *= 5.0f;
                    player.SetSpeed(player.speed);
                    InvalidateRect(hwnd, NULL, FALSE);
                }
            }
            if (wp == VK_SPACE) {
                player.speed *= -1;
                player.SetSpeed(player.speed);
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        if (wp == 81) {
            if (AttackChargeAmmount == 3) {
                AttackChargeAmmount = 0;
                Attack = TRUE;
                AttackTimeCount = 0;
                Score_Count += 200;
                boss.health -= 100;
                mciSendString(TEXT("open \"res//레이저1.wav\" type waveaudio alias sfx"), NULL, 0, NULL);
                mciSendString(TEXT("play sfx from 0"), NULL, 0, NULL);
                sound1 = true;
                sound1count = 10;
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        break;
    case WM_KEYUP:
        if (wp == VK_RETURN && player.Dash) {
            if (player.Dash) {
                player.Dash = FALSE;
                player.speed /= 5.0f;
                player.SetSpeed(player.speed);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lp);
        int y = HIWORD(lp);
        if (!GameStart) {
            if (!menu.Select_Difficulty) {
                if (x<menu.Start.right && x>menu.Start.left && y < menu.Start.bottom && y > menu.Start.top)  menu.Select_Difficulty = true;
                else if (x<menu.Start_EXIT.right && x>menu.Start_EXIT.left && y < menu.Start_EXIT.bottom && y > menu.Start_EXIT.top) menu.GameExit();
            }
            else if (GameEnd) {
                if (x<menu.END_EXIT.right && x>menu.END_EXIT.left && y < menu.END_EXIT.bottom && y > menu.END_EXIT.top) { menu.GameExit();}
                else if (x<menu.MENUBUTTON.right && x>menu.MENUBUTTON.left && y < menu.MENUBUTTON.bottom && y > menu.MENUBUTTON.top) {
                    KillTimer(hwnd, 1);
                    KillTimer(hwnd, 2);
                    KillTimer(hwnd, 3);
                    KillTimer(hwnd, 69);

                    // 게임 상태 완전 리셋
                    ResetGame(&spawncount,&timecount);

                    // 타이머 재설정
                    SetTimer(hwnd, 1, 32, NULL);
                    SetTimer(hwnd, 2, 50, NULL);
                    SetTimer(hwnd, 3, 1, NULL);

                    // 메뉴 상태로 복귀
                    GameEnd = false;
                    GameStart = false;
                    menu.Select_Difficulty = false;
                    selected_done = false;

                    InvalidateRect(hwnd, NULL, TRUE);  // 화면 갱신
                }
            }
            else
            {
                if (x<menu.EASY.right && x>menu.EASY.left && y < menu.EASY.bottom && y > menu.EASY.top) 
                { 
                    GameDifficulty = easy; selected_done = true; GameStart = true; player.health = 10; spikeManager.SetLevel(1); SetTimer(hwnd, 69, 1000, NULL);
                }
                else if (x<menu.NORMAL.right && x>menu.NORMAL.left && y < menu.NORMAL.bottom && y > menu.NORMAL.top) 
                { 
                    GameDifficulty = normal; selected_done = true; GameStart = true; player.health = 5; spikeManager.SetLevel(3); SetTimer(hwnd, 69, 1000, NULL);
                }
                else if (x<menu.HARD.right && x>menu.HARD.left && y < menu.HARD.bottom && y > menu.HARD.top) 
                { 
                    GameDifficulty = hard; selected_done = true; GameStart = true; player.health = 3; spikeManager.SetLevel(2); SetTimer(hwnd, 69, 1000, NULL); 
                }

                boss.Init(xx / 2, yy / 2, 100, spikeManager.GetLevel());    // 보스 초기화
            }
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
    }
    case WM_DESTROY:

        mciSendString(TEXT("close bgm"), NULL, 0, NULL);
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}