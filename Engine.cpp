#include "Engine.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum EBrickType
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN highlight_pen;
HPEN brick_red_pen;
HPEN brick_blue_pen;
HPEN platform_circle_pen;
HPEN platform_inner_pen;
HBRUSH highlight_brush;
HBRUSH brick_red_brush;
HBRUSH brick_blue_brush;
HBRUSH platform_circle_brush;
HBRUSH platform_inner_brush;

const int BRICK_WIDTH = 15;
const int BRICK_HEIGHT = 7;
const int CELL_WIDTH = 16;
const int CELL_HEIGHT = 8;
const int LEVEL_X_OFFSET = 8;
const int LEVEL_Y_OFFSET = 6;
const int CIRCLE_SIZE = 7;

int inner_width = 21;

char LEVEL_01[14][12] =
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

void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}

void Init()
{
   CreatePenBrush(255, 255, 255, highlight_pen, highlight_brush);
   CreatePenBrush(255, 85, 85, brick_red_pen, brick_red_brush);
   CreatePenBrush(85, 255, 255, brick_blue_pen, brick_blue_brush);
   CreatePenBrush(151, 0, 0, platform_circle_pen, platform_circle_brush);
   CreatePenBrush(0, 128, 192, platform_inner_pen, platform_inner_brush);
}

void DrawBrick(HDC hdc, int x, int y, EBrickType brick_type)
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
      x * GLOBAL_SCALE,
      y * GLOBAL_SCALE,
      (x + BRICK_WIDTH) * GLOBAL_SCALE,
      (y + BRICK_HEIGHT) * GLOBAL_SCALE,
      2 * GLOBAL_SCALE, 2 * GLOBAL_SCALE);
}

void SetBrickLetterColors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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

void DrawBrickLetter(HDC hdc, int x, int y, EBrickType brick_type, int rotation_step)
{
   bool switch_color;
   double offset;
   double rotation_angle;
   int brick_half_height = BRICK_HEIGHT * GLOBAL_SCALE / 2;
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
         y + brick_half_height - GLOBAL_SCALE,
         x + BRICK_WIDTH * GLOBAL_SCALE,
         y + brick_half_height);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc,
         x,
         y + brick_half_height,
         x + BRICK_WIDTH * GLOBAL_SCALE,
         y + brick_half_height + GLOBAL_SCALE - 1);
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

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)GLOBAL_SCALE;
      back_part_offset = (int)round(offset);

      Rectangle(hdc,
         0,
         -brick_half_height - back_part_offset,
         BRICK_WIDTH * GLOBAL_SCALE,
         brick_half_height - back_part_offset);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc,
         0,
         -brick_half_height,
         BRICK_WIDTH * GLOBAL_SCALE,
         brick_half_height);

      SetWorldTransform(hdc, &old_xform);
   }
}

void DrawLevel(HDC hdc)
{
   for (int i = 0; i < 14; i++)
      for (int j = 0; j < 12; j++)
      {
         DrawBrick(hdc, LEVEL_X_OFFSET + j * CELL_WIDTH, LEVEL_Y_OFFSET + i * CELL_HEIGHT, (EBrickType)LEVEL_01[i][j]);
      }
}

void DrawPlatform(HDC hdc, int x, int y)
{
   SelectObject(hdc, platform_circle_pen);
   SelectObject(hdc, platform_circle_brush);

   Ellipse(hdc,
      x * GLOBAL_SCALE,
      y * GLOBAL_SCALE,
      (x + CIRCLE_SIZE) * GLOBAL_SCALE,
      (y + CIRCLE_SIZE) * GLOBAL_SCALE);
   Ellipse(hdc,
      (x + 21) * GLOBAL_SCALE,
      y * GLOBAL_SCALE,
      (x + CIRCLE_SIZE + inner_width) * GLOBAL_SCALE,
      (y + CIRCLE_SIZE) * GLOBAL_SCALE);

   SelectObject(hdc, highlight_pen);
   SelectObject(hdc, highlight_brush);

   Arc(hdc,
      (x + 1) * GLOBAL_SCALE,
      (y + 1) * GLOBAL_SCALE,
      (x + CIRCLE_SIZE - 1) * GLOBAL_SCALE,
      (y + CIRCLE_SIZE - 1) * GLOBAL_SCALE,
      (x + 1 + 1) * GLOBAL_SCALE,
      (y + 1) * GLOBAL_SCALE,
      (x + 1) * GLOBAL_SCALE,
      (y + 1 + 2) * GLOBAL_SCALE);

   SelectObject(hdc, platform_inner_pen);
   SelectObject(hdc, platform_inner_brush);

   RoundRect(hdc,
      (x + 4) * GLOBAL_SCALE,
      (y + 1) * GLOBAL_SCALE,
      (x + 4 + inner_width - 1) * GLOBAL_SCALE,
      (y + 1 + 5) * GLOBAL_SCALE,
      3 * GLOBAL_SCALE,
      3 * GLOBAL_SCALE);
}

void DrawFrame(HDC hdc)
{
  // DrawLevel(hdc);
   //DrawPlatform(hdc, 50, 100);

   for (int i = 0; i < 16; i++)
   {
      DrawBrickLetter(hdc,20 + i*CELL_WIDTH*GLOBAL_SCALE, 100, EBT_Blue, i);
      DrawBrickLetter(hdc,20 + i*CELL_WIDTH*GLOBAL_SCALE, 130, EBT_Red, i);
   }
}