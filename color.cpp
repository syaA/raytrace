
#include "color.h"

#include <cmath>

sRGBColor srgb_from_xyz(const XYZColor& xyz)
{
  // XYZ→sRGB
  sRGBColor srgb;
  srgb.r = +3.240479*xyz.x - 1.537150*xyz.y - 0.498535*xyz.z;
  srgb.g = -0.969256*xyz.x + 1.875992*xyz.y + 0.041556*xyz.z;
  srgb.b = +0.055648*xyz.x - 0.204043*xyz.y + 1.057311*xyz.z;

  return srgb;
}

RGBColor rgb_from_srgb(const sRGBColor& srgb)
{
  // ガンマ調整
  RGBColor rgb;
  rgb.r = std::pow(srgb.r, 0.45f);
  rgb.g = std::pow(srgb.g, 0.45f);
  rgb.b = std::pow(srgb.b, 0.45f);

  return rgb;
}

