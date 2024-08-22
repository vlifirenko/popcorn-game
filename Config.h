#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

class Color
{
public:
   Color(unsigned char r, unsigned char g, unsigned char b);

   unsigned char R, G, B;
};

class Config
{
public:
   static void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
   static void CreatePenBrush(const Color& color, HPEN& pen, HBRUSH& brush);

   static const Color RED_BRICK_COLOR, BLUE_BRICK_COLOR;

   static const int GLOBAL_SCALE = 3;
   static const int BORDER_X_OFFSET = 6;
   static const int BORDER_Y_OFFSET = 4;
   static const int LEVEL_WIDTH = 12;
   static const int LEVEL_HEIGHT = 14;
   static const int LEVEL_X_OFFSET = 8;
   static const int LEVEL_Y_OFFSET = 6;
   static const int CELL_WIDTH = 16;
   static const int CELL_HEIGHT = 8;
   static const int BALL_SIZE = 4;
   static const int MAX_X_POS = LEVEL_X_OFFSET + CELL_WIDTH * LEVEL_WIDTH;
   static const int MAX_Y_POS = 199 - BALL_SIZE;
   static const int PLATFORM_Y_POS = 185;
   static const int CIRCLE_SIZE = 7;
   static const int FPS = 20;
   static const int BRICK_WIDTH = 15;
   static const int BRICK_HEIGHT = 7;
   static const int RED_R = 255;
   static const int RED_G = 85;
   static const int RED_B = 85;
   static const int BLUE_R = 85;
   static const int BLUE_G = 255;
   static const int BLUE_B = 255;
};