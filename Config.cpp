#include "Config.h"

const Color Config::RED_BRICK_COLOR(255, 85, 85);
const Color Config::BLUE_BRICK_COLOR(85, 255, 255);

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