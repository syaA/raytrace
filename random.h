#ifndef INCLUDED_RANODM_H
#define INCLUDED_RANODM_H

#include "type.h"
#include "vec.h"
#include "util.h"

struct xorshift_engine
{
  xorshift_engine()
    : x(123456789), y(362436069), z(521288629), w(88675123)
  {}
  xorshift_engine(unsigned x, unsigned y, unsigned z, unsigned w)
    : x(x), y(y), z(z), w(w)
  {}

  unsigned generate()
  {
    unsigned t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
  }
private:
  unsigned x, y, z, w;
};

// [0, 1]
value_type rand_01();

// 0 ~ n-1
inline int rand_num(int n)
{
  return static_cast<int>(rand_01() * (n - 1));
}
inline vector3 rand_vector()
{
  return normalize(vector3(rand_01(), rand_01(), rand_01()));
}
inline vector3 rand_on_sphere()
{
  value_type theta = M_PI * rand_01();
  value_type phi = 2 * M_PI * rand_01();
  value_type s = std::sin(theta);
  return vector3(s*std::cos(phi), s*std::sin(phi), std::cos(theta));
}

#endif // ndef INCLUDED_RANODM_H
