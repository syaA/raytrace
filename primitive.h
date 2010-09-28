#ifndef INCLUDED_PRIMITIVE_H
#define INCLUDED_PRIMITIVE_H

#include "type.h"
#include "vec.h"

class Sphere
{
public:
  Sphere(const vector3& position, value_type radius)
    : pos_(position), r_(radius)
  {}

  const vector3& position() const { return pos_; }
  value_type radius() const { return r_; }

  vector3 normal(const vector3& pos) const {
    return normalize(pos - pos_);
  }
private:
  vector3 pos_;
  value_type r_;
};

class Plane
{
public:
  Plane(const vector3& pos, const vector3& normal)
    : pos_(pos), nml_(normal)
  {}

  const vector3& position() const { return pos_; }
  const vector3& normal() const { return nml_; }

  const vector3& normal(const vector3&) const {
    return nml_;
  }
private:
  vector3 pos_;
  vector3 nml_;
};

class Triangle
{
public:
  Triangle(const vector3 *pos, const vector3 *nml)
  {
    for (int i=0; i<3; ++i) {
      pos_[i] = pos[i];
      nml_[i] = nml[i];
    }
  }
  Triangle(const vector3& p0, const vector3& n0,
           const vector3& p1, const vector3& n1,
           const vector3& p2, const vector3& n2)
  {
    pos_[0] = p0; pos_[1] = p1; pos_[2] = p2;
    nml_[0] = n0; nml_[1] = n1; nml_[2] = n2;
  }

  const vector3& position(int index) const { return pos_[index]; }
  vector3 normal(const vector3& p) const
  {
    value_type u = dot(p - pos_[0], normalize(pos_[2] - pos_[0]));
    value_type v = dot(p - pos_[0], normalize(pos_[1] - pos_[0]));
    return normalize(bary_centric(nml_[0], nml_[1], nml_[2], u, v));
  }
  
private:
  vector3 pos_[3];
  vector3 nml_[3];
};

#endif // ndef INCLUDED_PRIMITIVE_H
