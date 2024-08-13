#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

enum EKeyType
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

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

const int GLOBAL_SCALE = 3;
const int TIMER_ID = WM_USER + 1;

class Engine
{
public:
   Engine();

   void InitEngine(HWND hwnd);
   void DrawFrame(HDC hdc, RECT& paint_area);
   int OnKeyDown(EKeyType keyType);
   int OnTimer();

   static const int LEVEL_WIDTH = 12;
   static const int LEVEL_HEIGHT = 14;

private:
   void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
   void RedrawPlatform();
   void DrawBrick(HDC hdc, int x, int y, EBrickType brick_type);
   void SetBrickLetterColors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
   void DrawBrickLetter(HDC hdc, int x, int y, EBrickType brick_type, ELetterType letter_type, int rotation_step);
   void DrawLevel(HDC hdc);
   void DrawPlatform(HDC hdc, int x, int y);
   void DrawBall(HDC hdc);
   void DrawBorder(HDC hdc, int x, int y, bool top_border);
   void DrawBounds(HDC hdc);
   void CheckLevelBrickHit(int& next_y_pos);
   void MoveBall();

   HWND hwnd;
   HPEN highlight_pen;
   HPEN letter_pen;
   HPEN brick_red_pen;
   HPEN brick_blue_pen;
   HPEN platform_circle_pen;
   HPEN platform_inner_pen;
   HPEN bg_pen;
   HPEN ball_pen;
   HPEN border_blue_pen;
   HPEN border_white_pen;
   HBRUSH brick_red_brush;
   HBRUSH brick_blue_brush;
   HBRUSH platform_circle_brush;
   HBRUSH platform_inner_brush;
   HBRUSH bg_brush;
   HBRUSH ball_brush;
   HBRUSH border_blue_brush;
   HBRUSH border_white_brush;

   int inner_width;
   int platform_x_pos;
   int platform_x_step;
   int platform_width;

   int ball_x_pos, ball_y_pos;
   double ball_speed, ball_direction;

   RECT platform_rect, prev_platform_rect, level_rect, ball_rect, prev_ball_rect;

   static const int BRICK_WIDTH = 15;
   static const int BRICK_HEIGHT = 7;
   static const int CELL_WIDTH = 16;
   static const int CELL_HEIGHT = 8;
   static const int LEVEL_X_OFFSET = 8;
   static const int LEVEL_Y_OFFSET = 6;
   static const int CIRCLE_SIZE = 7;
   static const int PLATFORM_Y_POS = 185;
   static const int PLATFORM_HEIGHT = 7;
   static const int BALL_SIZE = 4;
   static const int MAX_X_POS = LEVEL_X_OFFSET + CELL_WIDTH * LEVEL_WIDTH;
   static const int MAX_Y_POS = 199 - BALL_SIZE;
   static const int BORDER_X_OFFSET = 6;
   static const int BORDER_Y_OFFSET = 4;
};