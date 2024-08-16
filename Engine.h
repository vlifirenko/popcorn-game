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

const int TIMER_ID = WM_USER + 1;

class Engine;
class Level;
class Platform;

class Ball
{
public:
   Ball();

   void Init();
   void Draw(HDC hdc, RECT& paint_area, Engine *engine);
   void Move(Engine* engine, Level* level, Platform* platform);

   HPEN bg_pen;
   HBRUSH bg_brush;
   HPEN ball_pen;
   HBRUSH ball_brush;
   double ball_speed;

   static const int BALL_SIZE = 4;

private:
   int ball_x_pos, ball_y_pos;
   double ball_direction;
   
   RECT ball_rect, prev_ball_rect;
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

class Platform
{
public:
   Platform();

   void Init();
   void Redraw(HWND hwnd);
   void Draw(HDC hdc, Engine* engine, RECT& paint_area);

   int x_pos;
   int x_step;
   int width;

   static const int PLATFORM_Y_POS = 185;

private:
   int inner_width;

   RECT platform_rect, prev_platform_rect;

   HPEN platform_circle_pen;
   HPEN platform_inner_pen;
   HPEN highlight_pen;
   HBRUSH platform_circle_brush;
   HBRUSH platform_inner_brush;

   static const int PLATFORM_HEIGHT = 7;
};

class Border
{
public:
   void Init();
   void DrawBounds(HDC hdc, Engine* engine);

   static const int BORDER_X_OFFSET = 6;
   static const int BORDER_Y_OFFSET = 4;
private:
   void DrawElement(HDC hdc, int x, int y, bool top_border, Engine* engine);

   HPEN border_blue_pen;
   HPEN border_white_pen;
   HBRUSH border_blue_brush;
   HBRUSH border_white_brush;
};

class Engine
{
public:
   Engine();

   void InitEngine(HWND hwnd);
   void DrawFrame(HDC hdc, RECT& paint_area);
   int OnKeyDown(EKeyType keyType);
   int OnTimer();

   static void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

   HWND hwnd;
   
   HPEN bg_pen;
   HBRUSH bg_brush;

   static const int GLOBAL_SCALE = 3; 
   static const int MAX_X_POS = Level::LEVEL_X_OFFSET + Level::CELL_WIDTH * Level::LEVEL_WIDTH;
   static const int MAX_Y_POS = 199 - Ball::BALL_SIZE;     
   static const int CIRCLE_SIZE = 7;

private:
   Ball ball;
   Level level;
   Platform platform;
   Border border;
};