#pragma once

#include "Config.h"
#include "ActiveBrick.h"

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
   void Draw(HWND hwnd, HDC hdc, RECT& paint_area);

   ActiveBrick active_brick;

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

   static char LEVEL_01[Config::LEVEL_HEIGHT][Config::LEVEL_WIDTH];

   static const int MAX_FADE_STEP = 10;
};