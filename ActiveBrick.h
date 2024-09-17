#pragma once

#include "Config.h"

enum EBrickType
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

class ActiveBrick
{
public:
   ActiveBrick(EBrickType brick_type);

   void Draw(HWND hwnd, HDC hdc, RECT& paint_area);
   void Act(HWND hwnd);

   static void SetupColors();
   static unsigned char GetFadingChannel(unsigned char color, unsigned char bg_color, int step);
   static void GetFadingColor(const Color& color, int step, HPEN& pen, HBRUSH& brush);

private:
   EBrickType brick_type;
   int fade_step;
   RECT brick_rect;

   static const int MAX_FADE_STEP = 80;
   
   static HPEN fading_red_brick_pens[MAX_FADE_STEP];
   static HBRUSH fading_red_brick_brushes[MAX_FADE_STEP];
   static HPEN fading_blue_brick_pens[MAX_FADE_STEP];
   static HBRUSH fading_blue_brick_brushes[MAX_FADE_STEP];
};