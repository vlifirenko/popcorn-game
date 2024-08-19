#include "Ball.h"

Ball::Ball()
   : ball_pen(0), ball_brush(0), bg_pen(0), bg_brush(0), ball_x_pos(20), ball_y_pos(170), ball_speed(3.0), ball_direction(M_PI - M_PI_4),
   ball_rect{}, prev_ball_rect{}
{
}

void Ball::Init()
{
   Config::CreatePenBrush(255, 255, 255, ball_pen, ball_brush);
}

void Ball::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &ball_rect))
      return;

   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);

   Ellipse(hdc,
      prev_ball_rect.left,
      prev_ball_rect.top,
      prev_ball_rect.right - 1,
      prev_ball_rect.bottom - 1);

   SelectObject(hdc, ball_pen);
   SelectObject(hdc, ball_brush);

   Ellipse(hdc,
      ball_rect.left,
      ball_rect.top,
      ball_rect.right - 1,
      ball_rect.bottom - 1);
}

void Ball::Move(HWND hwnd, Level* level, int platform_x_pos, int platform_width)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = Config::MAX_X_POS - Config::BALL_SIZE;
   int platform_y_pos = Config::PLATFORM_Y_POS - Config::BALL_SIZE;

   prev_ball_rect = ball_rect;

   next_x_pos = ball_x_pos + (int)(ball_speed * cos(ball_direction));
   next_y_pos = ball_y_pos - (int)(ball_speed * sin(ball_direction));

   if (next_x_pos < Config::BORDER_X_OFFSET)
   {
      next_x_pos = Config::LEVEL_X_OFFSET - (next_x_pos - Config::LEVEL_X_OFFSET);
      ball_direction = M_PI - ball_direction;
   }

   if (next_y_pos < Config::BORDER_Y_OFFSET)
   {
      next_y_pos = Config::BORDER_Y_OFFSET - (next_y_pos - Config::BORDER_Y_OFFSET);
      ball_direction = -ball_direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      ball_direction = M_PI - ball_direction;
   }

   if (next_y_pos > Config::MAX_Y_POS)
   {
      next_y_pos = Config::MAX_Y_POS - (next_y_pos - Config::MAX_Y_POS);
      ball_direction = M_PI + (M_PI - ball_direction);
   }

   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         ball_direction = M_PI + (M_PI - ball_direction);
      }
   }

   level->CheckLevelBrickHit(next_y_pos, ball_direction);

   ball_x_pos = next_x_pos;
   ball_y_pos = next_y_pos;

   ball_rect.left = ball_x_pos * Config::GLOBAL_SCALE;
   ball_rect.top = ball_y_pos * Config::GLOBAL_SCALE;
   ball_rect.right = ball_rect.left + Config::BALL_SIZE * Config::GLOBAL_SCALE;
   ball_rect.bottom = ball_rect.top + Config::BALL_SIZE * Config::GLOBAL_SCALE;

   InvalidateRect(hwnd, &prev_ball_rect, FALSE);
   InvalidateRect(hwnd, &ball_rect, FALSE);
}