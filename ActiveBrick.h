#pragma once

#include "Config.h"

class ActiveBrick
{
public:
   ActiveBrick();

   void Draw(HWND hwnd, HDC hdc, RECT& paint_area);
   void Act(HWND hwnd);

private:
   int fade_step;
   RECT brick_rect;

   static const int MAX_FADE_STEP = 80;
};