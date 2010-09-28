
#include "put_pixel.h"
#include "util.h"

void put_pixel(SDL_Surface *surface, int x, int y, int r, int g, int b)
{
  r = clamp(r, 0, 0xff);
  g = clamp(g, 0, 0xff);
  b = clamp(b, 0, 0xff);
  Uint32 color = SDL_MapRGB(surface->format, r, g, b);
  int bpp = surface->format->BytesPerPixel;
  /* この p は取得したいピクセルを指すアドレス */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = color;
    break;
  case 2:
    *(Uint16 *)p = color;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (color >> 16) & 0xff;
      p[1] = (color >> 8) & 0xff;
      p[2] = color & 0xff;
    } else {
      p[0] = color & 0xff;
      p[1] = (color >> 8) & 0xff;
      p[2] = (color >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32*)p = color;
    break;
  }
}
