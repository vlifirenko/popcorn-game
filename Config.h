#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

class Config
{
public:
   static void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

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
};