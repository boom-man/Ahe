#pragma once
#include <windows.h>

class Menu
{


public:
	HBITMAP START_MENUBIT, START_STARTBIT, START_EXITBIT, EASYBIT, NORMALBIT, HARDBIT;
	void GameExit();
	void GameStart();
	void MenuDraw(HDC hdc);
	BOOL Select_Difficulty;
	RECT Start = { 1300,500,1700,700 };
	RECT EXIT = { 1300,700,1700,1100 };
	RECT EASY = { 1300,300,1700,500 };
	RECT NORMAL = { 1300,500,1700,700 };
	RECT HARD = { 1300,700,1700,900 };

};