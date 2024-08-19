#pragma once

#include "Config.h"
#include "Level.h"

class Ball
{
public:
   Ball();

   void Init();
   void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);
   void Move(HWND hwnd, Level* level, int platform_x_pos, int platform_width);

   HPEN bg_pen;
   HBRUSH bg_brush;
   HPEN ball_pen;
   HBRUSH ball_brush;
   double ball_speed;

private:
   int ball_x_pos, ball_y_pos;
   double ball_direction;

   RECT ball_rect, prev_ball_rect;
};