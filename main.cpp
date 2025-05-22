#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "player.h"
#include "boss.h"
#include "map.h"
#include "mapstyle0.h"
#include "SpikeManager.h"
Player player;
Boss boss;
Map map;
MapStyle0 style0;
SpikeManager spikeManager;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GameInit();
void GameUpdate(HWND hwnd);
void GameRender(HDC hdc);

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
        0, 0, 1024, 900, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

void GameInit() {
    map.Init(400,300, 250); // �� �߽� ������
    player.Init(400, 300,100);  // �÷��̾� �ʱ�ȭ
    boss.Init(100, 100, 200);    // ���� �ʱ�ȭ
}

void GameUpdate(HWND hwnd, int timecount) {
    player.Update();  // �÷��̾� ������Ʈ
    boss.Update(player, timecount);    // ���� ������Ʈ
    map.Update();     //�� ������Ʈ        
    spikeManager.Update(boss,player);
    // ���� ���� ������Ʈ (�浹 ����, ������ ȹ�� ��)
}

void GameRender(HDC hdc) {
    map.Draw(hdc);
    player.Draw(hdc);  // �÷��̾� �׸���
    boss.Draw(hdc);    // ���� �׸���
    spikeManager.Draw(hdc);
    TCHAR buffer[64];
    wsprintf(buffer, TEXT("SpikeCount: %d"), spikeManager.spikeCount);
    TextOut(hdc, 10, 10, buffer, lstrlen(buffer)); // ��ǥ (10, 10)�� ���
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    PAINTSTRUCT ps;
    HDC hdc,mdc, hMemDC; // ������� �� ��Ʈ�ʿ�
    HBITMAP hBitmap, OldBit[3]; // ��Ʈ�ʿ�
    BITMAP bmp; // ��Ʈ�ʿ�
    RECT rt;// ���� ���ۿ�
    static HBITMAP  Yongbit[11], hBit; // ��ǳ ��Ʈ�� �����
    static RECT rectView; // ��Ʈ�� ��
    static int YongW, YongH;

    HBRUSH hBrush, oldBrush; // �귯��

    static int timecount = 0, spawncount = 0;
    static float p_speed;
    switch (msg) {
    case WM_CREATE:
        //============================================================================
        hdc = GetDC(hwnd);
        GetClientRect(hwnd, &rectView);
        for (int i = 0; i < 11; i++) {
            Yongbit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(101 + i));
        }
        GetObject(Yongbit[0], sizeof(BITMAP), &bmp);
        YongW = bmp.bmWidth; YongH = bmp.bmHeight;
        //=============================================================================
        SetTimer(hwnd, 1, 60, NULL);
        SetTimer(hwnd, 2, 50, NULL);
        map.SetStyle(&style0);
        GameInit();
        //==============================================================================
        p_speed = 1.0f;
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

        GameRender(mdc);
        spikeManager.yong.Draw(mdc,hMemDC,Yongbit,OldBit,YongW,YongH);

        //===================================================================
        BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
        DeleteDC(mdc);
        DeleteObject(hBitmap);
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_TIMER:
        if (wp == 1) {
            timecount++;
            GameUpdate(hwnd, timecount);  // Ÿ�̸ӿ� ���� ���� ������Ʈ
            InvalidateRect(hwnd, NULL, FALSE);  // ȭ�� ���� ��ħ
            break;
        }
        if (wp == 2) {
            spawncount++;
            spikeManager.SpawnManager(boss,player,spawncount);
        }
    case WM_KEYDOWN:
        if (wp == VK_RETURN) {
            if (!player.Dash) {
                player.Dash = TRUE;
                p_speed *= 5.0f;
                player.SetSpeed(p_speed);
            }
        }
        if (wp == VK_SPACE) {
            p_speed *= -1;
            player.SetSpeed(p_speed);
        }
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case WM_KEYUP:
        if (wp == VK_RETURN) {
            if (player.Dash) {
                player.Dash = FALSE;
                p_speed /= 5.0f;
                player.SetSpeed(p_speed);
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