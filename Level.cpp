#include "Level.h"

char Level::LEVEL_01[Config::LEVEL_HEIGHT][Config::LEVEL_WIDTH] =
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

Level::Level()
   : fade_step(0), brick_red_pen(0), brick_blue_pen(0), letter_pen(0), brick_red_brush(0), brick_blue_brush(0), level_rect{}
{
}

void Level::Init()
{
   letter_pen = CreatePen(PS_SOLID, Config::GLOBAL_SCALE, RGB(255, 255, 255));

   Config::CreatePenBrush(255, 85, 85, brick_red_pen, brick_red_brush);
   Config::CreatePenBrush(85, 255, 255, brick_blue_pen, brick_blue_brush);

   level_rect.left = Config::LEVEL_X_OFFSET * Config::GLOBAL_SCALE;
   level_rect.top = Config::LEVEL_Y_OFFSET * Config::GLOBAL_SCALE;
   level_rect.right = level_rect.left + Config::CELL_WIDTH * Config::LEVEL_WIDTH * Config::GLOBAL_SCALE;
   level_rect.bottom = level_rect.top + Config::CELL_HEIGHT * Config::LEVEL_HEIGHT * Config::GLOBAL_SCALE;
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
      x * Config::GLOBAL_SCALE,
      y * Config::GLOBAL_SCALE,
      (x + BRICK_WIDTH) * Config::GLOBAL_SCALE,
      (y + BRICK_HEIGHT) * Config::GLOBAL_SCALE,
      2 * Config::GLOBAL_SCALE, 2 * Config::GLOBAL_SCALE);
}

void Level::SetBrickLetterColors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
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
   int brick_half_height = BRICK_HEIGHT * Config::GLOBAL_SCALE / 2;
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
         y + brick_half_height - Config::GLOBAL_SCALE,
         x + BRICK_WIDTH * Config::GLOBAL_SCALE,
         y + brick_half_height);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc,
         x,
         y + brick_half_height,
         x + BRICK_WIDTH * Config::GLOBAL_SCALE,
         y + brick_half_height + Config::GLOBAL_SCALE - 1);
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

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Config::GLOBAL_SCALE;
      back_part_offset = (int)round(offset);

      Rectangle(hdc,
         0,
         -brick_half_height - back_part_offset,
         BRICK_WIDTH * Config::GLOBAL_SCALE,
         brick_half_height - back_part_offset);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc,
         0,
         -brick_half_height,
         BRICK_WIDTH * Config::GLOBAL_SCALE,
         brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, letter_pen);
            Ellipse(hdc,
               0 + 5 * Config::GLOBAL_SCALE,
               (-5 * Config::GLOBAL_SCALE) / 2,
               0 + 10 * Config::GLOBAL_SCALE,
               5 * Config::GLOBAL_SCALE / 2);
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}

void Level::Draw(HWND hwnd, HDC hdc, RECT& paint_area)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &level_rect))
      return;

   RECT brick_rect;
   HPEN pen;
   HBRUSH brush;

   for (int i = 0; i < Config::LEVEL_WIDTH; i++)
      for (int j = 0; j < Config::LEVEL_HEIGHT; j++)
      {
         DrawBrick(hdc, Config::LEVEL_X_OFFSET + j * Config::CELL_WIDTH, Config::LEVEL_Y_OFFSET + i * Config::CELL_HEIGHT, (EBrickType)LEVEL_01[i][j]);
      }

   Config::CreatePenBrush(
      85 - fade_step * (85 / MAX_FADE_STEP),
      255 - fade_step * (255 / MAX_FADE_STEP),
      255 - fade_step * (255 / MAX_FADE_STEP),
      pen, brush);

   ++fade_step;

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   brick_rect.left = (Config::LEVEL_X_OFFSET + 1 * Config::CELL_WIDTH) * Config::GLOBAL_SCALE;
   brick_rect.top = (Config::LEVEL_Y_OFFSET + 1 * Config::CELL_HEIGHT) * Config::GLOBAL_SCALE;
   brick_rect.right = brick_rect.left + BRICK_WIDTH * Config::GLOBAL_SCALE;
   brick_rect.bottom = brick_rect.top + BRICK_HEIGHT * Config::GLOBAL_SCALE;

   InvalidateRect(hwnd, &brick_rect, TRUE);

   RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right, brick_rect.bottom,
      2 * Config::GLOBAL_SCALE, 2 * Config::GLOBAL_SCALE);
}

void Level::CheckLevelBrickHit(int& next_y_pos, double& ball_direction)
{
   int brick_y_pos = Config::LEVEL_Y_OFFSET + Config::LEVEL_HEIGHT * Config::CELL_HEIGHT;

   for (int i = Config::LEVEL_HEIGHT - 1; i >= 0; i--)
   {
      for (int j = 0; j < Config::LEVEL_WIDTH; j++)
      {
         if (LEVEL_01[i][j] == 0)
            continue;

         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            ball_direction = -ball_direction;
         }
      }

      brick_y_pos -= Config::CELL_HEIGHT;
   }
}