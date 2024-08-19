#pragma once

#include <Windows.h>

class Config
{
public:
   static void CreatePenBrush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

   static const int GLOBAL_SCALE = 3;
};