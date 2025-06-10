#pragma once
#include <windows.h>

class Menu
{


public:
	HBITMAP START_MENUBIT, START_STARTBIT, START_EXITBIT, EASYBIT, NORMALBIT, HARDBIT, EndBit, MenuButtonBit;
	void GameExit();
	void GameStart();
	void MenuDraw(HDC hdc);
	void MenuEndDraw(HDC hdc);
	BOOL Select_Difficulty;
	RECT Start = { 1300,500,1700,700 };
	RECT Start_EXIT = { 1300,700,1700,1100 };
	RECT EASY = { 1300,300,1700,500 };
	RECT NORMAL = { 1300,500,1700,700 };
	RECT HARD = { 1300,700,1700,900 };
	RECT MENUBUTTON = { 1180,820,1580,1020 };
	RECT END_EXIT = { 780,820,1180,1020 };

};