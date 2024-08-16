#include "Engine.h"

char LEVEL_01[Level::LEVEL_HEIGHT][Level::LEVEL_WIDTH] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,
   1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,
   2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,
   1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,
   2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0
};

Engine::Engine()
{
}

Ball::Ball()
   : ball_x_pos(20), ball_y_pos(170), ball_speed(3.0), ball_direction(M_PI - M_PI_4)
{
}

Level::Level()
{
}

void Engine::CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}

Platform::Platform()
   : x_pos(Border::BORDER_X_OFFSET), x_step(Engine::GLOBAL_SCALE), width(28), inner_width(21)
{
}

void Platform::Init()
{
   highlight_pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   Engine::CreatePenBrush(151, 0, 0, platform_circle_pen, platform_circle_brush);
   Engine::CreatePenBrush(0, 128, 192, platform_inner_pen, platform_inner_brush);
}

void Platform::Redraw(HWND hwnd)
{
   prev_platform_rect = platform_rect;

   platform_rect.left = (Level::LEVEL_X_OFFSET + x_pos) * Engine::GLOBAL_SCALE;
   platform_rect.top = PLATFORM_Y_POS * Engine::GLOBAL_SCALE;
   platform_rect.right = platform_rect.left + width * Engine::GLOBAL_SCALE;
   platform_rect.bottom = platform_rect.top + PLATFORM_HEIGHT * Engine::GLOBAL_SCALE;

   InvalidateRect(hwnd, &prev_platform_rect, FALSE);
   InvalidateRect(hwnd, &platform_rect, FALSE);
}

void Engine::InitEngine(HWND h)
{
   hwnd = h;

   CreatePenBrush(15, 63, 31, bg_pen, bg_brush);

   ball.Init();
   level.Init();
   platform.Init();
   border.Init();

   platform.Redraw(hwnd);

   SetTimer(hwnd, TIMER_ID, 50, 0);
}

void Level::Init()
{
   letter_pen = CreatePen(PS_SOLID, Engine::GLOBAL_SCALE, RGB(255, 255, 255));

   Engine::CreatePenBrush(255, 85, 85, brick_red_pen, brick_red_brush);
   Engine::CreatePenBrush(85, 255, 255, brick_blue_pen, brick_blue_brush);

   level_rect.left = Level::LEVEL_X_OFFSET * Engine::GLOBAL_SCALE;
   level_rect.top = Level::LEVEL_Y_OFFSET * Engine::GLOBAL_SCALE;
   level_rect.right = level_rect.left + Level::CELL_WIDTH * Level::LEVEL_WIDTH * Engine::GLOBAL_SCALE;
   level_rect.bottom = level_rect.top + Level::CELL_HEIGHT * Level::LEVEL_HEIGHT * Engine::GLOBAL_SCALE;
}

void Ball::Init()
{
   Engine::CreatePenBrush(255, 255, 255, ball_pen, ball_brush);
}

void Border::Init()
{
   Engine::CreatePenBrush(85, 255, 255, border_blue_pen, border_blue_brush);
   Engine::CreatePenBrush(255, 255, 255, border_white_pen, border_white_brush);
}

void Level::DrawBrick(HDC hdc, int x, int y, EBrickType brick_type)
{
   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;

   case EBT_Red:
      pen = brick_red_pen;
      brush = brick_red_brush;
      break;

   case EBT_Blue:
      pen = brick_blue_pen;
      brush = brick_blue_brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc,
      x * Engine::GLOBAL_SCALE,
      y * Engine::GLOBAL_SCALE,
      (x + BRICK_WIDTH) * Engine::GLOBAL_SCALE,
      (y + BRICK_HEIGHT) * Engine::GLOBAL_SCALE,
      2 * Engine::GLOBAL_SCALE, 2 * Engine::GLOBAL_SCALE);
}

void Level::SetBrickLetterColors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
   if (is_switch_color)
   {
      front_pen = brick_red_pen;
      front_brush = brick_red_brush;

      back_pen = brick_blue_pen;
      back_brush = brick_blue_brush;
   }
   else {
      front_pen = brick_blue_pen;
      front_brush = brick_blue_brush;

      back_pen = brick_red_pen;
      back_brush = brick_red_brush;
   }
}

void Level::DrawBrickLetter(HDC hdc, int x, int y, EBrickType brick_type, ELetterType letter_type, int rotation_step)
{
   bool switch_color;
   double offset;
   double rotation_angle;
   int brick_half_height = BRICK_HEIGHT * Engine::GLOBAL_SCALE / 2;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
      return;

   rotation_step %= 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16 * (double)(8 - rotation_step);

   if (rotation_step > 4 && rotation_step <= 12)
      switch_color = brick_type == EBT_Blue;
   else
      switch_color = brick_type == EBT_Red;
   SetBrickLetterColors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)
   {
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc,
         x,
         y + brick_half_height - Engine::GLOBAL_SCALE,
         x + BRICK_WIDTH * Engine::GLOBAL_SCALE,
         y + brick_half_height);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc,
         x,
         y + brick_half_height,
         x + BRICK_WIDTH * Engine::GLOBAL_SCALE,
         y + brick_half_height + Engine::GLOBAL_SCALE - 1);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);

      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)brick_half_height;

      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Engine::GLOBAL_SCALE;
      back_part_offset = (int)round(offset);

      Rectangle(hdc,
         0,
         -brick_half_height - back_part_offset,
         BRICK_WIDTH * Engine::GLOBAL_SCALE,
         brick_half_height - back_part_offset);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc,
         0,
         -brick_half_height,
         BRICK_WIDTH * Engine::GLOBAL_SCALE,
         brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, letter_pen);
            Ellipse(hdc,
               0 + 5 * Engine::GLOBAL_SCALE,
               (-5 * Engine::GLOBAL_SCALE) / 2,
               0 + 10 * Engine::GLOBAL_SCALE,
               5 * Engine::GLOBAL_SCALE / 2);
         }
      }   

      SetWorldTransform(hdc, &old_xform);
   }
}

void Level::DrawLevel(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &level_rect))
      return;

   for (int i = 0; i < LEVEL_WIDTH; i++)
      for (int j = 0; j < LEVEL_HEIGHT; j++)
      {
         DrawBrick(hdc, LEVEL_X_OFFSET + j * CELL_WIDTH, LEVEL_Y_OFFSET + i * CELL_HEIGHT, (EBrickType)LEVEL_01[i][j]);
      }
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
      x * Engine::GLOBAL_SCALE,
      y * Engine::GLOBAL_SCALE,
      (x + Engine::CIRCLE_SIZE) * Engine::GLOBAL_SCALE,
      (y + Engine::CIRCLE_SIZE) * Engine::GLOBAL_SCALE);
   Ellipse(hdc,
      (x + 21) * Engine::GLOBAL_SCALE,
      y * Engine::GLOBAL_SCALE,
      (x + Engine::CIRCLE_SIZE + inner_width) * Engine::GLOBAL_SCALE,
      (y + Engine::CIRCLE_SIZE) * Engine::GLOBAL_SCALE);

   SelectObject(hdc, highlight_pen);

   Arc(hdc,
      (x + 1) * Engine::GLOBAL_SCALE,
      (y + 1) * Engine::GLOBAL_SCALE,
      (x + Engine::CIRCLE_SIZE - 1) * Engine::GLOBAL_SCALE,
      (y + Engine::CIRCLE_SIZE - 1) * Engine::GLOBAL_SCALE,
      (x + 1 + 1) * Engine::GLOBAL_SCALE,
      (y + 1) * Engine::GLOBAL_SCALE,
      (x + 1) * Engine::GLOBAL_SCALE,
      (y + 1 + 2) * Engine::GLOBAL_SCALE);

   SelectObject(hdc, platform_inner_pen);
   SelectObject(hdc, platform_inner_brush);

   RoundRect(hdc,
      (x + 4) * Engine::GLOBAL_SCALE,
      (y + 1) * Engine::GLOBAL_SCALE,
      (x + 4 + inner_width - 1) * Engine::GLOBAL_SCALE,
      (y + 1 + 5) * Engine::GLOBAL_SCALE,
      3 * Engine::GLOBAL_SCALE,
      3 * Engine::GLOBAL_SCALE);
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

void Border::DrawElement(HDC hdc, int x, int y, bool top_border, Engine *engine)
{
   SelectObject(hdc, border_blue_pen);
   SelectObject(hdc, border_blue_brush);

   if (top_border)
   {
      Rectangle(hdc,
         (x + 0) * Engine::GLOBAL_SCALE,
         (y + 1) * Engine::GLOBAL_SCALE,
         (x + 4) * Engine::GLOBAL_SCALE,
         (y + 4) * Engine::GLOBAL_SCALE
      );
   }
   else
   {
      Rectangle(hdc,
         (x + 1) * Engine::GLOBAL_SCALE,
         y * Engine::GLOBAL_SCALE,
         (x + 4) * Engine::GLOBAL_SCALE,
         (y + 4) * Engine::GLOBAL_SCALE
      );
   }

   SelectObject(hdc, border_white_pen);
   SelectObject(hdc, border_white_brush);

   if (top_border)
   {
      Rectangle(hdc,
         x * Engine::GLOBAL_SCALE,
         y * Engine::GLOBAL_SCALE,
         (x + 4) * Engine::GLOBAL_SCALE,
         (y + 1) * Engine::GLOBAL_SCALE
      );
   }
   else
   {
      Rectangle(hdc,
         x * Engine::GLOBAL_SCALE,
         y * Engine::GLOBAL_SCALE,
         (x + 1) * Engine::GLOBAL_SCALE,
         (y + 4) * Engine::GLOBAL_SCALE
      );
   }

   SelectObject(hdc, engine->bg_pen);
   SelectObject(hdc, engine->bg_brush);

   if (top_border)
   {
      Rectangle(hdc,
         (x + 2) * Engine::GLOBAL_SCALE,
         (y + 2) * Engine::GLOBAL_SCALE,
         (x + 3) * Engine::GLOBAL_SCALE,
         (y + 3) * Engine::GLOBAL_SCALE
      );
   }
   else
   {
      Rectangle(hdc,
         (x + 2) * Engine::GLOBAL_SCALE,
         (y + 1) * Engine::GLOBAL_SCALE,
         (x + 3) * Engine::GLOBAL_SCALE,
         (y + 2) * Engine::GLOBAL_SCALE
      );
   }
}

void Border::DrawBounds(HDC hdc, Engine* engine)
{
   for (int i = 0; i < 50; i++)
      DrawElement(hdc, 2, 1 + i * 4, false, engine);

   for (int i = 0; i < 50; i++)
      DrawElement(hdc, 201, 1 + i * 4, false, engine);

   for (int i = 0; i < 50; i++)
      DrawElement(hdc, 3 + i * 4, 0, true, engine);
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

   border.DrawBounds(hdc, this);
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

void Level::CheckLevelBrickHit(int &next_y_pos, double &ball_direction)
{
   int brick_y_pos = LEVEL_Y_OFFSET + LEVEL_HEIGHT * CELL_HEIGHT;

   for (int i = LEVEL_HEIGHT - 1; i >= 0; i--)
   {
      for (int j = 0; j < LEVEL_WIDTH; j++)
      {
         if (LEVEL_01[i][j] == 0)
            continue;

         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            ball_direction = -ball_direction;
         }
      }

      brick_y_pos -= CELL_HEIGHT;
   }
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

   ball_rect.left = ball_x_pos * Engine::GLOBAL_SCALE;
   ball_rect.top = ball_y_pos * Engine::GLOBAL_SCALE;
   ball_rect.right = ball_rect.left + BALL_SIZE * Engine::GLOBAL_SCALE;
   ball_rect.bottom = ball_rect.top + BALL_SIZE * Engine::GLOBAL_SCALE;

   InvalidateRect(engine->hwnd, &prev_ball_rect, FALSE);
   InvalidateRect(engine->hwnd, &ball_rect, FALSE);
}

int Engine::OnTimer()
{
   ball.Move(this, &level, &platform);

   return 0;
}