#include <Windows.h>
#include "Spike.h"

Yong::Yong() : x(0), y(0),gap(0), attack(false), spawncount(0), attackpoint(0) {}

int A[4] = {0,1,2,3};
				// 시작 위치, 간격, 몇초 동안 알려줄건지, 방향 = 가로 세로선2개씩, 레벨, 딜레이
void Yong::Spawn(float startX, float startGap, int spawn, int vector, int Dlay) {
	attack = true;
	x = startX;
	gap = startGap;
	spawncount = spawn;
	bitmapcount = 0;
	BangHyang = vector;
	Delay = Dlay;
	if (attackpoint == 0 ) {
		if (BangHyang == 0) {
			x = 160;
			y = 1080 - 40;
		}
		else {
			x = 160;
			y = 0;
		}
	}
	else if (attackpoint == 1) {
		if (BangHyang == 0) {
			x = 1920 / 2;
			y = 1080 - 40;
		}
		else {
			x = 1920 / 2;
			y = 0;
		}
	}
	else if (attackpoint == 2) {
		if (BangHyang == 0) {
			x = 1920 - 40;
			y = 0;
		}
		else {
			x = 0;
			y = 0;
		}
	}
	else {
		if (BangHyang == 0) {
			x = 1920 - 40;
			y = 1080 / 2;
		}
		else {
			x = 0;
			y = 1080 / 2;
		}
	}
}
void Yong::Update(Player& player, int w[], int h[]) {
	if (attack) {
		if (Delay > 0) {
			Delay--;
			return;
		}
		if (x < 0 || x > 1920 || y < 0 || y >  1080) {
			attack = false;
			return;
		}
		if (spawncount > 0) {
			spawncount--;
			// 아직 무지개 연출 중 → 움직이지 않음
			return;
		}
		// 충돌 체크


		if (attackpoint == 0 || attackpoint == 1) {
			attackRect.left = x;
			attackRect.top = y;
			attackRect.right = attackRect.left + 40 * 19;
			attackRect.bottom = attackRect.top + 40;
		}
		else {
			attackRect.left = x;
			attackRect.top = y;
			attackRect.right = x + 40;
			attackRect.bottom = y + 40 * 12;
		}

		if (CheckCollision(attackRect, player.x, player.y)) {
			player.health--;
			player.delay = 10;
		}
		// 방향에 따라 이동 처리
		if (attackpoint == 0 || attackpoint == 1){
			if (BangHyang == 0) {//up
				y -= 10;
			}
			else {//down
				y += 10;
			}
		}
		else {
			if (BangHyang == 0) {//left
				x -= 10;
			}
			else {//right
				x += 10;
			}
		}
	}
}
void Yong::DrawRainbowLines(HDC hdc, int alpha,int attackpoint) {
	int count = 20;
	int spacing = 40;
	int lineWidth;
	int lineHeight;
	int startX;
	int startY;
	if (attackpoint == 0 || attackpoint == 1) {
		lineWidth = 5;
		lineHeight = 1080;
	}
	else {
		lineWidth = 1920;
		lineHeight = 5;
	}
	if (attackpoint == 0 || attackpoint == 2) {
		startX = (1920 / 2) - (count * spacing);
		startY = (1080 / 2) - (count * spacing);
	}
	else {
		startX = (1920 / 2);
		startY = (1080 / 2);
	}
	int y = 0;

	COLORREF rainbowColors[7] = {
		RGB(255, 0, 0), RGB(255, 127, 0), RGB(255, 255, 0),
		RGB(0, 255, 0), RGB(0, 0, 255), RGB(75, 0, 130),
		RGB(148, 0, 211)
	};

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, (BYTE)alpha, 0 };
	HDC memDC = CreateCompatibleDC(hdc);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = lineWidth;
	bmi.bmiHeader.biHeight = -lineHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* bits;
	HBITMAP hBmp = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
	if (!memDC) {
		OutputDebugString(L"CreateCompatibleDC failed\n");
	}
	HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBmp);
	if (!hBmp || !bits) {
		OutputDebugString(L"CreateDIBSection failed or bits is null\n");
	}

	for (int i = 0; i < count; i++) {
		COLORREF c = rainbowColors[i&7];
		if (bits) {
			for (int py = 0; py < lineHeight; py++) {
				for (int px = 0; px < lineWidth; px++) {
					((DWORD*)bits)[py * lineWidth + px] = c;
				}
			}
		}
		if (attackpoint == 0 || attackpoint == 1) {
			int drawX = startX + i * spacing;
			AlphaBlend(hdc, drawX, y, lineWidth, lineHeight, memDC, 0, 0, lineWidth, lineHeight, bf);
		}
		else {
			int drawY = startY + i * spacing;
			AlphaBlend(hdc, 0, drawY, lineWidth, lineHeight, memDC, 0, 0, lineWidth, lineHeight, bf);
		}
	}

	SelectObject(memDC, oldBmp);
	DeleteObject(hBmp);
	DeleteDC(memDC);
}
void Yong::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w[], int h[]) {
	if (attack && Delay <=0) {
		// 비트맵 출력 (방향은 BangHyang)
		OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[attackpoint]);
		if (attackpoint == 1 || attackpoint == 0) {
			for (int i = 0; i < 19; i++) {
				TransparentBlt(hdc, x + 40 * i, y, w[attackpoint] / 3, h[attackpoint] / 3, hMemDC, 0, 0, w[attackpoint], h[attackpoint], RGB(0, 0, 0));
			}
		}
		else {
			for (int i = 0; i < 12; i++) {
				TransparentBlt(hdc, x, y + 40 * i, w[attackpoint] / 3, h[attackpoint] / 3, hMemDC, 0, 0, w[attackpoint], h[attackpoint], RGB(0, 0, 0));
			}
		}
		SelectObject(hMemDC, OldBit[2]);
		if (spawncount != 0) {
			DrawRainbowLines(hdc, rainbowAlpha,attackpoint);
			// alpha 값을 증가시켜 점점 보이게 함
			if (rainbowAlpha < 210)
				rainbowAlpha += 5;  // 클래스 멤버로 둬야 함
		}
	}
}
bool Yong::CheckCollision(const RECT& rect, float px, float py) {
	return (px >= rect.left && px <= rect.right &&
		py >= rect.top && py <= rect.bottom);
}