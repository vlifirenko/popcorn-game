#include "Engine.h"


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
   DrawLevel(hdc);
   DrawPlatform(hdc, 50, 100);
}