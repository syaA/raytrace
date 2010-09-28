#ifndef INCLUDED_RAY_H
#define INCLUDED_RAY_H

#include "type.h"
#include "vec.h"
#include "spectrum.h"

class Sphere;
class Plane;
class Triangle;

class Ray
{
public:
  Ray(const vector3 org, const vector3 dir)
    : org_(org), dir_(dir),
      decay_(Spectrum::one()), achive_emissive_(false),
      pick_wavelength_(-1)
  {}

  const vector3& origin() const { return org_; }
  const vector3& direction() const { return dir_; }
  const Spectrum& decay() const { return decay_; }
  const Spectrum& spectrum() const { return decay_; }
  bool achive_emissive() const { return achive_emissive_; }

  vector3 position(value_type t) const { return org_ + t * dir_; }
  
  void reflect(
    const vector3& pos, const vector3& dir, const Spectrum& decay)
  {
    org_ = pos;
    dir_ = dir;
    decay_ *= decay;
  }
  void refract(
    const vector3& pos,
    const vector3& dir,
    const Spectrum& decay,
    value_type eta)
  {
    org_ = pos;
    dir_ = dir;
    decay_ *= decay;
#if 0
    decay_.shift(
      Spectrum::wavelength_from_index(pick_wavelength_) * (1/eta - 1));
    for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) {
      if (decay_[i] > 0) {
        pick_wavelength_ = i;
        break;
      }
    }
#endif
  }
  void achieve_emissive(const Spectrum& spectrum)
  {
    decay_ = spectrum * decay_;
    achive_emissive_ = true;
  }
  void absorb()
  {
    decay_.zeroise();
  }
  int pick_wavelength(int index)
  {
    if (pick_wavelength_ >= 0) {
      return pick_wavelength_;
    }

    pick_wavelength_ = index;
    for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) {
      if (i != index) {
        decay_[i] = 0;
      }
    }
    decay_[pick_wavelength_] *= Spectrum::NUM_SAMPLE;
    return pick_wavelength_;
  }
private:
  vector3 org_;
  vector3 dir_;
  Spectrum decay_;
  bool achive_emissive_;
  int pick_wavelength_;
};


value_type intersect(const Ray& ray, const Sphere& s);
value_type intersect(const Ray& ray, const Plane& s);
value_type intersect(const Ray& ray, const Triangle& s);

#endif // ndef INCLUDED_RAY_H
