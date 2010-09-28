
#include <cmath>

#include "ray.h"
#include "primitive.h"


namespace {
const value_type EPSILON = 1e-6;
} // end of anonymus namespace

value_type intersect(const Ray& ray, const Sphere& s)
{
  vector3 co = s.position() - ray.origin();
  value_type co2 = dot(co, co);
  value_type cod = dot(co, ray.direction());
  value_type D = cod * cod - (co2 - s.radius() * s.radius());
  if (D < 0) {
    return -1;
  }

  value_type sD = std::sqrt(D);
  value_type t0 = cod - sD;
  value_type t1 = cod + sD;
  return std::min(t0, t1);
}

value_type intersect(const Ray& ray, const Plane& plane)
{
  value_type d = -dot(plane.position(), plane.normal());
  value_type v = dot(ray.direction(), plane.normal());
  if (abs(v) < EPSILON) {
    return -1;
  } else {
    return -(dot(ray.origin(), plane.normal()) + d) / v;
  }
}

value_type intersect(const Ray& ray, const Triangle &t)
{
  vector3 e1 = t.position(1) - t.position(0);
  vector3 e2 = t.position(2) - t.position(0);

  vector3 pv = cross(ray.direction(), e2);
  value_type det = dot(e1, pv);

  if (det > -EPSILON && det < EPSILON) {
    return -1;
  }
  value_type inv_det = 1 / det;

  vector3 tv = ray.origin() - t.position(0);
  value_type u = dot(tv, pv) * inv_det;
  if (u < 0.0 || u > 1.0) {
    return -1;
  }

  vector3 qv = cross(tv, e1);
  value_type v = dot(ray.direction(), qv) * inv_det;
  if (v < 0.0 || v > 1.0) {
    return -1;
  }

  if (u + v > 1) {
    return -1;
  }
  
  return dot(e2, qv) * inv_det;
}
