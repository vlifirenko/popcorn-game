#include "ActiveBrick.h"

ActiveBrick::ActiveBrick()
   :fade_step(0)
{
}

void ActiveBrick::Draw(HWND hwnd, HDC hdc, RECT& paint_area)
{
   HPEN pen;
   HBRUSH brush;

   Config::CreatePenBrush(
      85 - fade_step * (85 / MAX_FADE_STEP),
      255 - fade_step * (255 / MAX_FADE_STEP),
      255 - fade_step * (255 / MAX_FADE_STEP),
      pen, brush);

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
   if (fade_step < MAX_FADE_STEP)
   {
      ++fade_step;
      InvalidateRect(hwnd, &brick_rect, TRUE);
   }
}