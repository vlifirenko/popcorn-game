#include "Config.h"

const Color Config::BG_COLOR(15, 63, 31);
const Color Config::RED_BRICK_COLOR(255, 85, 85);
const Color Config::BLUE_BRICK_COLOR(85, 255, 255);

HPEN Config::bg_pen;
HBRUSH Config::bg_brush;

void Config::SetupColors()
{
   Config::CreatePenBrush(Config::BG_COLOR, bg_pen, bg_brush);
}

void Config::CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}

void Config::CreatePenBrush(const Color& color, HPEN& pen, HBRUSH& brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
   brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
   : R(r), G(g), B(b)
{
}