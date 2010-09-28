#ifndef INCLUDED_VEC_H
#define INCLUDED_VEC_H

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

#include "type.h"

template<class BaseT>
struct vec3
{
  typedef BaseT value_type;
  
  vec3() {}
  vec3(BaseT x, BaseT y, BaseT z) : x(x), y(y), z(z) {}
  vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

  vec3& operator=(const vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
  const vec3 operator+() const { return *this; }
  const vec3 operator-() const { return vec3(-x, -y, -z); }

  static vec3 replicate(BaseT v) { return vec3(v, v, v); }
  static vec3 zero() { return replicate(0); }
  static vec3 one() { return replicate(1); }
  
  union
  {
    struct
    {
      BaseT x, y, z;
    };
    BaseT v[3];
  };
};

#define DEF_VEC3_OP(op) \
  template<class BaseT> \
  vec3<BaseT>& operator op=(vec3<BaseT>& a, const vec3<BaseT>& b) \
  { \
    a.x op= b.x; \
    a.y op= b.y; \
    a.z op= b.z; \
    return a; \
  } \
  template<class BaseT> \
  const vec3<BaseT> operator op(const vec3<BaseT>& a, const vec3<BaseT>& b) \
  { \
    vec3<BaseT> r(a); \
    r op= b; \
    return r; \
  }
DEF_VEC3_OP(+)
DEF_VEC3_OP(-)
DEF_VEC3_OP(*)
DEF_VEC3_OP(/)
#undef DEF_VEC3_OP

#define DEF_VEC3_OP_S(op) \
  template<class BaseT> \
  vec3<BaseT>& operator op=(vec3<BaseT>& a, BaseT b) \
  { \
    a.x op= b; \
    a.y op= b; \
    a.z op= b; \
    return a; \
  } \
  template<class BaseT> \
  const vec3<BaseT> operator op(const vec3<BaseT>& a, BaseT b) \
  { \
    vec3<BaseT> r(a); \
    r op= b; \
    return r; \
  }
DEF_VEC3_OP_S(*)
DEF_VEC3_OP_S(/)
#undef DEF_VEC3_OP_S

template<class BaseT>
vec3<BaseT> operator*(BaseT a, const vec3<BaseT>& b)
{
  vec3<BaseT> r(b);
  r *= a;
  return r;
}

template<class BaseT>
vec3<BaseT> operator==(const vec3<BaseT>& a, const vec3<BaseT>& b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}
template<class BaseT>
vec3<BaseT> operator!=(const vec3<BaseT>& a, const vec3<BaseT>& b)
{
  return !(a == b);
}

template<class BaseT>
const BaseT *as_array(const vec3<BaseT>& v)
{
  return v.v;
}
template<class BaseT>
const std::string to_string(const vec3<BaseT>& v)
{
  std::stringstream ss;
  ss << "[3](" << v.x << ", " << v.y << ", " << v.z << ")";
  return ss.str();
}
template<class BaseT>
std::ostream& operator<<(std::ostream& o, const vec3<BaseT>& v)
{
  return o << to_string(v);
}

template<class BaseT>
BaseT dot(const vec3<BaseT>& a, const vec3<BaseT>& b)
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
template<class BaseT>
vec3<BaseT> cross(const vec3<BaseT>& a, const vec3<BaseT>& b)
{
  return vec3<BaseT>(
    a.y*b.z - a.z*b.y,
    a.z*b.x - a.x*b.z,
    a.x*b.y - a.y*b.x);
}

template<class BaseT>
BaseT norm_sq(const vec3<BaseT>& v)
{
  return dot(v, v);
}
template<class BaseT>
BaseT norm(const vec3<BaseT>& v)
{
  return std::sqrt(norm_sq(v));
}
template<class BaseT>
vec3<BaseT> normalize(const vec3<BaseT>& v)
{
  return v / norm(v);
}
template<class BaseT>
vec3<BaseT> orthograph(const vec3<BaseT>& v, const vec3<BaseT>& n)
{
  return dot(v, n) * n;
}
template<class BaseT>
vec3<BaseT> reflect(const vec3<BaseT>& v, const vec3<BaseT>& n)
{
  return v - 2 * dot(v, n) * n;
}
template<class BaseT>
vec3<BaseT> refract(
  const vec3<BaseT>& v, const vec3<BaseT>& n, value_type rel_eta)
{
  value_type vn = dot(v, n);
  return
    -std::sqrt(1 - rel_eta * rel_eta * (1 - vn * vn)) * n +
      rel_eta * (v - vn * n);
}

template<class BaseT>
vec3<BaseT> abs(const vec3<BaseT>& v)
{
  return vec3<BaseT>(std::abs(v.x),
                     std::abs(v.y),
                     std::abs(v.z));
}

template<class BaseT>
vec3<BaseT> bary_centric(
  const vec3<BaseT>& v0,
  const vec3<BaseT>& v1,
  const vec3<BaseT>& v2,
  BaseT f,
  BaseT g)
{
  return (1 - f - g) * v0 + f * v1 + g * v2;
}

typedef vec3<value_type> vector3;

#endif // def INCLUDED_VEC_H
