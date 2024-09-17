#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

enum EKeyType
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int TIMER_ID = WM_USER + 1;

class Engine
{
public:
   Engine();

   void InitEngine(HWND hwnd);
   void DrawFrame(HDC hdc, RECT& paint_area);
   int OnKeyDown(EKeyType keyType);
   int OnTimer();

private:
   HWND hwnd;

   Ball ball;
   Level level;
   Platform platform;
   Border border;
};