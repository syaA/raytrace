#ifndef INCLUDED_COLOR_H
#define INCLUDED_COLOR_H

#include "type.h"

// sRGB
struct sRGBColor
{
  value_type r, g, b;
};

// XYZ
struct XYZColor
{
  value_type x, y, z;
};

// ガンマ修正済
struct RGBColor
{
  value_type r, g, b;
};


sRGBColor srgb_from_xyz(const XYZColor& xyz);
RGBColor rgb_from_srgb(const sRGBColor& srgb);

#endif // ndef INCLUDED_COLOR_H
