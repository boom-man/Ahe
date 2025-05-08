#include <windows.h>
#include "player.h"
#include "boss.h"
#include "map.h"
#include "mapstyle0.h"
Player player;
Boss boss;
Map map;
MapStyle0 style0;
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
        0, 0, xx, yy, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

void GameInit() {
    map.Init(xx / 2, yy / 2, 250); // �� �߽� ������
    player.Init(xx/2, yy/2,100);  // �÷��̾� �ʱ�ȭ
    boss.Init(100, 100, 200);    // ���� �ʱ�ȭ
}

void GameUpdate(HWND hwnd) {
    player.Update();  // �÷��̾� ������Ʈ
    boss.Update(player);    // ���� ������Ʈ
    map.Update();     //�� ������Ʈ
    // ���� ���� ������Ʈ (�浹 ����, ������ ȹ�� ��)
}

void GameRender(HDC hdc) {
    map.Draw(hdc);
    player.Draw(hdc);  // �÷��̾� �׸���
    boss.Draw(hdc);    // ���� �׸���
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    PAINTSTRUCT ps;
    HDC hdc,mdc;
    HBITMAP hBitmap;
    RECT rt;
    HBRUSH hBrush, oldBrush;


    static float p_speed;
    switch (msg) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 60, NULL);
        map.SetStyle(&style0);
        GameInit();

        p_speed = 1.0f;
        break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rt);
        mdc = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
        SelectObject(mdc, (HBITMAP)hBitmap);
        hBrush = CreateSolidBrush(RGB(255,255, 255));
        FillRect(mdc, &rt, hBrush);
        oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
        SelectObject(mdc, oldBrush);
        DeleteObject(hBrush);

        GameRender(mdc);
        BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
        DeleteDC(mdc);
        DeleteObject(hBitmap);
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_TIMER:
        GameUpdate(hwnd);  // Ÿ�̸ӿ� ���� ���� ������Ʈ
        InvalidateRect(hwnd, NULL, FALSE);  // ȭ�� ���� ��ħ
        break;

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