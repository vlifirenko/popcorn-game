#include "Engine.h"

Engine::Engine()
   : bg_brush(0), bg_pen(0), hwnd(0)
{
}

Ball::Ball()
   : ball_pen(0), ball_brush(0), bg_pen(0), bg_brush(0), ball_x_pos(20), ball_y_pos(170), ball_speed(3.0), ball_direction(M_PI - M_PI_4),
   ball_rect{}, prev_ball_rect{}
{
}

Platform::Platform()
   : highlight_pen(0), platform_circle_brush(0), platform_circle_pen(0), platform_inner_brush(0), platform_inner_pen(0), 
   x_pos(Border::BORDER_X_OFFSET), x_step(Config::GLOBAL_SCALE), width(28), inner_width(21), platform_rect{}, prev_platform_rect{}
{
}

void Platform::Init()
{
   highlight_pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   Config::CreatePenBrush(151, 0, 0, platform_circle_pen, platform_circle_brush);
   Config::CreatePenBrush(0, 128, 192, platform_inner_pen, platform_inner_brush);
}

void Platform::Redraw(HWND hwnd)
{
   prev_platform_rect = platform_rect;

   platform_rect.left = (Level::LEVEL_X_OFFSET + x_pos) * Config::GLOBAL_SCALE;
   platform_rect.top = PLATFORM_Y_POS * Config::GLOBAL_SCALE;
   platform_rect.right = platform_rect.left + width * Config::GLOBAL_SCALE;
   platform_rect.bottom = platform_rect.top + PLATFORM_HEIGHT * Config::GLOBAL_SCALE;

   InvalidateRect(hwnd, &prev_platform_rect, FALSE);
   InvalidateRect(hwnd, &platform_rect, FALSE);
}

void Engine::InitEngine(HWND h)
{
   hwnd = h;

   Config::CreatePenBrush(15, 63, 31, bg_pen, bg_brush);

   ball.Init();
   level.Init();
   platform.Init();
   border.Init();

   platform.Redraw(hwnd);

   SetTimer(hwnd, TIMER_ID, 50, 0);
}



void Ball::Init()
{
   Config::CreatePenBrush(255, 255, 255, ball_pen, ball_brush);
}

void Platform::Draw(HDC hdc, Engine* engine, RECT& paint_area)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &platform_rect))
      return;

   int x = Level::LEVEL_X_OFFSET + x_pos;
   int y = PLATFORM_Y_POS;

   SelectObject(hdc, engine->bg_pen);
   SelectObject(hdc, engine->bg_brush);

   Rectangle(hdc,
      prev_platform_rect.left,
      prev_platform_rect.top,
      prev_platform_rect.right,
      prev_platform_rect.bottom);

   SelectObject(hdc, platform_circle_pen);
   SelectObject(hdc, platform_circle_brush);

   Ellipse(hdc,
      x * Config::GLOBAL_SCALE,
      y * Config::GLOBAL_SCALE,
      (x + Engine::CIRCLE_SIZE) * Config::GLOBAL_SCALE,
      (y + Engine::CIRCLE_SIZE) * Config::GLOBAL_SCALE);
   Ellipse(hdc,
      (x + 21) * Config::GLOBAL_SCALE,
      y * Config::GLOBAL_SCALE,
      (x + Engine::CIRCLE_SIZE + inner_width) * Config::GLOBAL_SCALE,
      (y + Engine::CIRCLE_SIZE) * Config::GLOBAL_SCALE);

   SelectObject(hdc, highlight_pen);

   Arc(hdc,
      (x + 1) * Config::GLOBAL_SCALE,
      (y + 1) * Config::GLOBAL_SCALE,
      (x + Engine::CIRCLE_SIZE - 1) * Config::GLOBAL_SCALE,
      (y + Engine::CIRCLE_SIZE - 1) * Config::GLOBAL_SCALE,
      (x + 1 + 1) * Config::GLOBAL_SCALE,
      (y + 1) * Config::GLOBAL_SCALE,
      (x + 1) * Config::GLOBAL_SCALE,
      (y + 1 + 2) * Config::GLOBAL_SCALE);

   SelectObject(hdc, platform_inner_pen);
   SelectObject(hdc, platform_inner_brush);

   RoundRect(hdc,
      (x + 4) * Config::GLOBAL_SCALE,
      (y + 1) * Config::GLOBAL_SCALE,
      (x + 4 + inner_width - 1) * Config::GLOBAL_SCALE,
      (y + 1 + 5) * Config::GLOBAL_SCALE,
      3 * Config::GLOBAL_SCALE,
      3 * Config::GLOBAL_SCALE);
}

void Ball::Draw(HDC hdc, RECT &paint_area, Engine *engine)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &ball_rect))
      return;

   SelectObject(hdc, engine->bg_pen);
   SelectObject(hdc, engine->bg_brush);

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

void Engine::DrawFrame(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   level.DrawLevel(hdc, paint_area);

   platform.Draw(hdc, this, paint_area);

   /*for (int i = 0; i < 16; i++)
   {
      DrawBrickLetter(hdc,20 + i*CELL_WIDTH*GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
      DrawBrickLetter(hdc,20 + i*CELL_WIDTH*GLOBAL_SCALE, 130, EBT_Red, ELT_O, i);
   }*/

   ball.Draw(hdc, paint_area, this);

   border.DrawBounds(hdc, bg_pen, bg_brush);
}

int Engine::OnKeyDown(EKeyType key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      platform.x_pos -= platform.x_step;

      if (platform.x_pos <= Border::BORDER_X_OFFSET)
         platform.x_pos = Border::BORDER_X_OFFSET;

      platform.Redraw(hwnd);
      break;
   case EKT_Right:
      platform.x_pos += platform.x_step;

      if (platform.x_pos >= MAX_X_POS - platform.width + 1)
         platform.x_pos = MAX_X_POS - platform.width + 1;

      platform.Redraw(hwnd);
      break;
   case EKT_Space:
      break;
   }

   return 0;
}

void Ball::Move(Engine* engine, Level* level, Platform* platform)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = Engine::MAX_X_POS - BALL_SIZE;
   int platform_y_pos = platform->PLATFORM_Y_POS - BALL_SIZE;

   prev_ball_rect = ball_rect;

   next_x_pos = ball_x_pos + (int)(ball_speed * cos(ball_direction));
   next_y_pos = ball_y_pos - (int)(ball_speed * sin(ball_direction));

   if (next_x_pos < Border::BORDER_X_OFFSET)
   {
      next_x_pos = Level::LEVEL_X_OFFSET - (next_x_pos - Level::LEVEL_X_OFFSET);
      ball_direction = M_PI - ball_direction;
   }

   if (next_y_pos < Border::BORDER_Y_OFFSET)
   {
      next_y_pos = Border::BORDER_Y_OFFSET - (next_y_pos - Border::BORDER_Y_OFFSET);
      ball_direction = -ball_direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      ball_direction = M_PI - ball_direction;
   }

   if (next_y_pos > Engine::MAX_Y_POS)
   {
      next_y_pos = Engine::MAX_Y_POS - (next_y_pos - Engine::MAX_Y_POS);
      ball_direction = M_PI + (M_PI - ball_direction);
   }

   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= platform->x_pos && next_x_pos <= platform->x_pos + platform->width)
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
   ball_rect.right = ball_rect.left + BALL_SIZE * Config::GLOBAL_SCALE;
   ball_rect.bottom = ball_rect.top + BALL_SIZE * Config::GLOBAL_SCALE;

   InvalidateRect(engine->hwnd, &prev_ball_rect, FALSE);
   InvalidateRect(engine->hwnd, &ball_rect, FALSE);
}

int Engine::OnTimer()
{
   ball.Move(this, &level, &platform);

   return 0;
}