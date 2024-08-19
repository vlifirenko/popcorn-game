#pragma once

#include "Config.h"

class Platform
{
public:
   Platform();

   void Init();
   void Redraw(HWND hwnd);
   void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);

   int x_pos;
   int x_step;
   int width;


private:
   int inner_width;

   RECT platform_rect, prev_platform_rect;

   HPEN platform_circle_pen;
   HPEN platform_inner_pen;
   HPEN highlight_pen;
   HBRUSH platform_circle_brush;
   HBRUSH platform_inner_brush;

   static const int PLATFORM_HEIGHT = 7;
};