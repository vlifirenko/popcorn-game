#pragma once

#include "Config.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum EBrickType
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

enum ELetterType
{
   ELT_None,
   ELT_O
};

class Level
{
public:
   Level();

   void Init();
   void CheckLevelBrickHit(int& next_y_pos, double& ball_direction);
   void DrawLevel(HDC hdc, RECT& paint_area);

   static const int LEVEL_WIDTH = 12;
   static const int LEVEL_HEIGHT = 14;
   static const int LEVEL_X_OFFSET = 8;
   static const int LEVEL_Y_OFFSET = 6;
   static const int CELL_WIDTH = 16;
   static const int CELL_HEIGHT = 8;

private:
   void DrawBrick(HDC hdc, int x, int y, EBrickType brick_type);
   void SetBrickLetterColors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
   void DrawBrickLetter(HDC hdc, int x, int y, EBrickType brick_type, ELetterType letter_type, int rotation_step);

   HPEN brick_red_pen;
   HPEN brick_blue_pen;
   HPEN letter_pen;
   HBRUSH brick_red_brush;
   HBRUSH brick_blue_brush;

   RECT level_rect;


   static const int BRICK_WIDTH = 15;
   static const int BRICK_HEIGHT = 7;
};