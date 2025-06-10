#include "Menu.h"
#include <cmath>
#include <cstdlib>

void Menu::GameExit() {
	PostQuitMessage(0);
}

void Menu::GameStart()
{

}

void Menu::MenuDraw(HDC hdc)
{
	HDC hdcmem = CreateCompatibleDC(hdc);
	SelectObject(hdcmem, START_MENUBIT);
	TransparentBlt(hdc, 0, 0, 1920, 1080, hdcmem, 0, 0, 1920, 1080, RGB(10, 10, 10));
	if (!Select_Difficulty)
	{
		SelectObject(hdcmem, START_STARTBIT);
		TransparentBlt(hdc, 1300, 500, 400, 200, hdcmem, 0, 0, 400, 200, RGB(0, 0, 0));

		SelectObject(hdcmem, START_EXITBIT);
		TransparentBlt(hdc, 1300, 700, 400, 200, hdcmem, 0, 0, 400, 200, RGB(0, 0, 0));
	}
	else if (Select_Difficulty)
	{
		SelectObject(hdcmem, EASYBIT);
		TransparentBlt(hdc, 1300, 300, 400, 200, hdcmem, 0, 0, 400, 200, RGB(0, 0, 0));

		SelectObject(hdcmem, NORMALBIT);
		TransparentBlt(hdc, 1300, 500, 400, 200, hdcmem, 0, 0, 400, 200, RGB(0, 0, 0));

		SelectObject(hdcmem, HARDBIT);
		TransparentBlt(hdc, 1300, 700, 400, 200, hdcmem, 0, 0, 400, 200, RGB(0, 0, 0));
	}

	DeleteDC(hdcmem);
}