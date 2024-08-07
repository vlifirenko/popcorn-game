#pragma once

#include <Windows.h>

enum EKeyType
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int GLOBAL_SCALE = 3;
const int TIMER_ID = WM_USER + 1;

void InitEngine(HWND hwnd);
void DrawFrame(HDC hdc, RECT& paint_area);
int OnKeyDown(EKeyType keyType);
int OnTimer();