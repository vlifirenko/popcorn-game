#include "Platform.h"

Platform::Platform()
   : highlight_pen(0), platform_circle_brush(0), platform_circle_pen(0), platform_inner_brush(0), platform_inner_pen(0),
   x_pos(Config::BORDER_X_OFFSET), x_step(Config::GLOBAL_SCALE), width(28), inner_width(21), platform_rect{}, prev_platform_rect{}
{
}

void Platform::Init()
{
   highlight_pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   Config::CreatePenBrush(151, 0, 0, platform_circle_pen, platform_circle_brush);
   Config::CreatePenBrush(0, 128, 192, platform_inner_pen, platform_inner_brush);
}

void Platform::Redraw(HWND hwnd)
{
   prev_platform_rect = platform_rect;

   platform_rect.left = (Config::LEVEL_X_OFFSET + x_pos) * Config::GLOBAL_SCALE;
   platform_rect.top = Config::PLATFORM_Y_POS * Config::GLOBAL_SCALE;
   platform_rect.right = platform_rect.left + width * Config::GLOBAL_SCALE;
   platform_rect.bottom = platform_rect.top + PLATFORM_HEIGHT * Config::GLOBAL_SCALE;

   InvalidateRect(hwnd, &prev_platform_rect, FALSE);
   InvalidateRect(hwnd, &platform_rect, FALSE);
}

void Platform::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &platform_rect))
      return;

   int x = Config::LEVEL_X_OFFSET + x_pos;
   int y = Config::PLATFORM_Y_POS;

   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);

   Rectangle(hdc,
      prev_platform_rect.left,
      prev_platform_rect.top,
      prev_platform_rect.right,
      prev_platform_rect.bottom);

   SelectObject(hdc, platform_circle_pen);
   SelectObject(hdc, platform_circle_brush);

   Ellipse(hdc,
      x * Config::GLOBAL_SCALE,
      y * Config::GLOBAL_SCALE,
      (x + Config::CIRCLE_SIZE) * Config::GLOBAL_SCALE,
      (y + Config::CIRCLE_SIZE) * Config::GLOBAL_SCALE);
   Ellipse(hdc,
      (x + 21) * Config::GLOBAL_SCALE,
      y * Config::GLOBAL_SCALE,
      (x + Config::CIRCLE_SIZE + inner_width) * Config::GLOBAL_SCALE,
      (y + Config::CIRCLE_SIZE) * Config::GLOBAL_SCALE);

   SelectObject(hdc, highlight_pen);

   Arc(hdc,
      (x + 1) * Config::GLOBAL_SCALE,
      (y + 1) * Config::GLOBAL_SCALE,
      (x + Config::CIRCLE_SIZE - 1) * Config::GLOBAL_SCALE,
      (y + Config::CIRCLE_SIZE - 1) * Config::GLOBAL_SCALE,
      (x + 1 + 1) * Config::GLOBAL_SCALE,
      (y + 1) * Config::GLOBAL_SCALE,
      (x + 1) * Config::GLOBAL_SCALE,
      (y + 1 + 2) * Config::GLOBAL_SCALE);

   SelectObject(hdc, platform_inner_pen);
   SelectObject(hdc, platform_inner_brush);

   RoundRect(hdc,
      (x + 4) * Config::GLOBAL_SCALE,
      (y + 1) * Config::GLOBAL_SCALE,
      (x + 4 + inner_width - 1) * Config::GLOBAL_SCALE,
      (y + 1 + 5) * Config::GLOBAL_SCALE,
      3 * Config::GLOBAL_SCALE,
      3 * Config::GLOBAL_SCALE);
}