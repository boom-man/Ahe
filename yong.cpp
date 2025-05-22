#include <Windows.h>
#include "Spike.h"


Yong::Yong() : x(0),gap(0), attack(false), spawncount(0), DC(0) {}

void Yong::Spawn(float startX, float startGap,  int spawn, int during) {
	attack = true;
	x = startX;
	gap = startGap;
	spawncount = spawn;
	bitmapcount = 0;
	DC = during;
}
void Yong::Update() {
	if (attack) {
		if (spawncount > 0) {
			spawncount--;
			return;
		}
		bitmapcount++;
		bitmapcount %= 11;
		if (DC > 0) {
			DC--;
		}
		else {
			attack = false;
		}
	}
}
void Yong::Draw(HDC hdc, HDC hMemDC, HBITMAP Yong[], HBITMAP OldBit[], int w, int h) {
	if (attack && spawncount == 0) {
		OldBit[2] = (HBITMAP)SelectObject(hMemDC, Yong[bitmapcount]);
		for (int i = 0; i < 10; i++) {
			// ���� ����� �����ϰ� �������� ���
			TransparentBlt(hdc, x, i*h, gap, h, hMemDC, 0, 0, w, h, RGB(255, 255, 255));
			// ���� ���� + ũ�� ����
			StretchBlt(hdc, x, i * h, gap, h, hMemDC, 0, 0, w, h, NOTSRCCOPY);
		}
		// ��Ʈ�� ���� ����
		SelectObject(hMemDC, OldBit[2]);
	}
}