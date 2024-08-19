#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h"

enum EKeyType
{
   EKT_Left,
   EKT_Right,
   EKT_Space
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

class Engine
{
public:
   Engine();

   void InitEngine(HWND hwnd);
   void DrawFrame(HDC hdc, RECT& paint_area);
   int OnKeyDown(EKeyType keyType);
   int OnTimer();

   HWND hwnd;
   
   HPEN bg_pen;
   HBRUSH bg_brush;

   static const int MAX_X_POS = Level::LEVEL_X_OFFSET + Level::CELL_WIDTH * Level::LEVEL_WIDTH;
   static const int MAX_Y_POS = 199 - Ball::BALL_SIZE;     
   static const int CIRCLE_SIZE = 7;

private:
   Ball ball;
   Level level;
   Platform platform;
   Border border;
};