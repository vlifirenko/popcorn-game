#include "Engine.h"


enum EBrickType
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN brick_red_pen;
HPEN brick_blue_pen;
HBRUSH brick_red_brush;
HBRUSH brick_blue_brush;

const int BRICK_WIDTH = 15;
const int BRICK_HEIGHT = 7;
const int CELL_WIDTH = 16;
const int CELL_HEIGHT = 8;
const int LEVEL_X_OFFSET = 8;
const int LEVEL_Y_OFFSET = 6;

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

void Init()
{
   brick_red_pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
   brick_red_brush = CreateSolidBrush(RGB(255, 85, 85));

   brick_blue_pen = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
   brick_blue_brush = CreateSolidBrush(RGB(85, 255, 255));
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

void DrawFrame(HDC hdc)
{
   DrawLevel(hdc);
}