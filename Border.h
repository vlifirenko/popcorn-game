#pragma once

#include "Config.h"

class Border
{
public:
   Border();

   void Init();
   void DrawBounds(HDC hdc, HPEN bg_pen, HBRUSH bg_brush);

private:
   void DrawElement(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush);

   HPEN border_blue_pen;
   HPEN border_white_pen;
   HBRUSH border_blue_brush;
   HBRUSH border_white_brush;
};