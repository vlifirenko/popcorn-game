#include "Engine.h"

Engine::Engine()
   : hwnd(0)
{
}

void Engine::InitEngine(HWND h)
{
   hwnd = h;

   ActiveBrick::SetupColors();

   ball.Init();
   level.Init();
   platform.Init();
   border.Init();

   platform.Redraw(hwnd);

   SetTimer(hwnd, TIMER_ID, 1000 / Config::FPS, 0);
}

void Engine::DrawFrame(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   level.Draw(hwnd, hdc, paint_area);

   platform.Draw(hdc, paint_area);

   /*for (int i = 0; i < 16; i++)
   {
      DrawBrickLetter(hdc,20 + i*CELL_WIDTH*GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
      DrawBrickLetter(hdc,20 + i*CELL_WIDTH*GLOBAL_SCALE, 130, EBT_Red, ELT_O, i);
   }*/

   ball.Draw(hdc, paint_area);

   border.DrawBounds(hdc);
}

int Engine::OnKeyDown(EKeyType key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      platform.x_pos -= platform.x_step;

      if (platform.x_pos <= Config::BORDER_X_OFFSET)
         platform.x_pos = Config::BORDER_X_OFFSET;

      platform.Redraw(hwnd);
      break;
   case EKT_Right:
      platform.x_pos += platform.x_step;

      if (platform.x_pos >= Config::MAX_X_POS - platform.width + 1)
         platform.x_pos = Config::MAX_X_POS - platform.width + 1;

      platform.Redraw(hwnd);
      break;
   case EKT_Space:
      break;
   }

   return 0;
}

int Engine::OnTimer()
{
   ball.Move(hwnd, &level, platform.x_pos, platform.width);

   level.active_brick.Act(hwnd);

   return 0;
}