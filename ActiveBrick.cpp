#include "ActiveBrick.h"

HPEN ActiveBrick::fading_red_brick_pens[MAX_FADE_STEP];
HBRUSH ActiveBrick::fading_red_brick_brushes[MAX_FADE_STEP];
HPEN ActiveBrick::fading_blue_brick_pens[MAX_FADE_STEP];
HBRUSH ActiveBrick::fading_blue_brick_brushes[MAX_FADE_STEP];

ActiveBrick::ActiveBrick(EBrickType brick_type)
   :fade_step(0), brick_type(brick_type)
{
}

void ActiveBrick::Draw(HWND hwnd, HDC hdc, RECT& paint_area)
{
   HPEN pen = 0;
   HBRUSH brush = 0;

   switch (brick_type)
   {
   case EBT_Red:
      pen = fading_red_brick_pens[fade_step];
      brush = fading_red_brick_brushes[fade_step];
      break;

   case EBT_Blue:
      pen = fading_blue_brick_pens[fade_step];
      brush = fading_blue_brick_brushes[fade_step];
      break;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   brick_rect.left = (Config::LEVEL_X_OFFSET + 1 * Config::CELL_WIDTH) * Config::GLOBAL_SCALE;
   brick_rect.top = (Config::LEVEL_Y_OFFSET + 1 * Config::CELL_HEIGHT) * Config::GLOBAL_SCALE;
   brick_rect.right = brick_rect.left + Config::BRICK_WIDTH * Config::GLOBAL_SCALE;
   brick_rect.bottom = brick_rect.top + Config::BRICK_HEIGHT * Config::GLOBAL_SCALE;


   RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right, brick_rect.bottom,
      2 * Config::GLOBAL_SCALE, 2 * Config::GLOBAL_SCALE);
}

void ActiveBrick::Act(HWND hwnd)
{
   if (fade_step < MAX_FADE_STEP - 1)
   {
      ++fade_step;
      InvalidateRect(hwnd, &brick_rect, TRUE);
   }
}

void ActiveBrick::SetupColors()
{
   for (int i = 0; i < MAX_FADE_STEP; i++)
   {
      GetFadingColor(Config::RED_BRICK_COLOR, i, fading_red_brick_pens[i], fading_red_brick_brushes[i]);
      GetFadingColor(Config::BLUE_BRICK_COLOR, i, fading_blue_brick_pens[i], fading_blue_brick_brushes[i]);
   }   
}

unsigned char ActiveBrick::GetFadingChannel(unsigned char color, unsigned char bg_color, int step)
{
   return color - step * (color - bg_color / MAX_FADE_STEP - 1);
}

void ActiveBrick::GetFadingColor(const Color &color, int step, HPEN &pen, HBRUSH &brush)
{
   Config::CreatePenBrush(
      GetFadingChannel(color.R, Config::BG_COLOR.R, step),
      GetFadingChannel(color.G, Config::BG_COLOR.G, step),
      GetFadingChannel(color.B, Config::BG_COLOR.B, step),
      pen,
      brush);
}