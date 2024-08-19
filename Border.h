#pragma once

#include <Windows.h>

class Config
{
public:
   static void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

   static const int GLOBAL_SCALE = 3;
};

class Border
{
public:
   Border();

   void Init();
   void DrawBounds(HDC hdc, HPEN bg_pen, HBRUSH bg_brush);

   static const int BORDER_X_OFFSET = 6;
   static const int BORDER_Y_OFFSET = 4;
private:
   void DrawElement(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush);

   HPEN border_blue_pen;
   HPEN border_white_pen;
   HBRUSH border_blue_brush;
   HBRUSH border_white_brush;
};