#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H


template<class ValueT>
ValueT clamp(ValueT x, ValueT min, ValueT max)
{
  return x < min ? min : x > max ? max : x;
}

template<class ValueT>
ValueT lerp(ValueT a, ValueT b, ValueT t)
{
  return a + (b - a) * t;
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // ndf M_PI

#endif // ndef INCLUDED_UTIL_H
