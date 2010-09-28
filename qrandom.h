#ifndef INCLUDED_QRANDOM_H
#define INCLUDED_QRANDOM_H

#include "type.h"

inline value_type vdC(int i, int base)
{
  value_type h = 0., f, factor;

  f = factor = 1. / base;

  while (i > 0) {
    h += i % base * factor;
    i /= base;
    factor *= f;
  }

  return h;
}

#endif //  INCLUDED_QRANDOM_H
