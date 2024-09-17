#include "Border.h"

Border::Border()
   : border_blue_pen(0), border_white_pen(0), border_blue_brush(0), border_white_brush(0)
{
}

void Border::Init()
{
   Config::CreatePenBrush(85, 255, 255, border_blue_pen, border_blue_brush);
   Config::CreatePenBrush(255, 255, 255, border_white_pen, border_white_brush);
}

void Border::DrawElement(HDC hdc, int x, int y, bool top_border)
{
   SelectObject(hdc, border_blue_pen);
   SelectObject(hdc, border_blue_brush);

   if (top_border)
   {
      Rectangle(hdc,
         (x + 0) * Config::GLOBAL_SCALE,
         (y + 1) * Config::GLOBAL_SCALE,
         (x + 4) * Config::GLOBAL_SCALE,
         (y + 4) * Config::GLOBAL_SCALE
      );
   }
   else
   {
      Rectangle(hdc,
         (x + 1) * Config::GLOBAL_SCALE,
         y * Config::GLOBAL_SCALE,
         (x + 4) * Config::GLOBAL_SCALE,
         (y + 4) * Config::GLOBAL_SCALE
      );
   }

   SelectObject(hdc, border_white_pen);
   SelectObject(hdc, border_white_brush);

   if (top_border)
   {
      Rectangle(hdc,
         x * Config::GLOBAL_SCALE,
         y * Config::GLOBAL_SCALE,
         (x + 4) * Config::GLOBAL_SCALE,
         (y + 1) * Config::GLOBAL_SCALE
      );
   }
   else
   {
      Rectangle(hdc,
         x * Config::GLOBAL_SCALE,
         y * Config::GLOBAL_SCALE,
         (x + 1) * Config::GLOBAL_SCALE,
         (y + 4) * Config::GLOBAL_SCALE
      );
   }

   SelectObject(hdc, Config::bg_pen);
   SelectObject(hdc, Config::bg_brush);

   if (top_border)
   {
      Rectangle(hdc,
         (x + 2) * Config::GLOBAL_SCALE,
         (y + 2) * Config::GLOBAL_SCALE,
         (x + 3) * Config::GLOBAL_SCALE,
         (y + 3) * Config::GLOBAL_SCALE
      );
   }
   else
   {
      Rectangle(hdc,
         (x + 2) * Config::GLOBAL_SCALE,
         (y + 1) * Config::GLOBAL_SCALE,
         (x + 3) * Config::GLOBAL_SCALE,
         (y + 2) * Config::GLOBAL_SCALE
      );
   }
}

void Border::DrawBounds(HDC hdc)
{
   for (int i = 0; i < 50; i++)
      DrawElement(hdc, 2, 1 + i * 4, false);

   for (int i = 0; i < 50; i++)
      DrawElement(hdc, 201, 1 + i * 4, false);

   for (int i = 0; i < 50; i++)
      DrawElement(hdc, 3 + i * 4, 0, true);
}