#ifndef INCLUDED_SPECTRUM_H
#define INCLUDED_SPECTRUM_H

#include <cstring>
#include <cmath>

#include "type.h"

class Spectrum
{
public:
  static const int NUM_SAMPLE = 16;
  static const value_type MIN_WAVELENGTH;
  static const value_type MAX_WAVELENGTH;

public:
  Spectrum() {}
  Spectrum(const value_type *sample)
  {
    set(sample);
  }
  Spectrum(const Spectrum& other)
  {
    set(other.sample_);
  }

  Spectrum& operator=(const Spectrum& other)
  {
    set(other.sample_);
    return *this;
  }

  void set(const value_type *sample)
  {
    std::memcpy(sample_, sample, sizeof(value_type) * NUM_SAMPLE);
  }

  const value_type operator[](int index) const { return sample_[index]; }
  value_type& operator[](int index) { return sample_[index]; }

  void shift(value_type wavelength)
  {
    value_type new_sample[NUM_SAMPLE];
    for (int i=0; i<NUM_SAMPLE; ++i) {
      int old_index =
        index_from_wavelength(wavelength_from_index(i)-wavelength);
      if (old_index < 0 ||
          old_index >= NUM_SAMPLE) {
        new_sample[i] = 0;
      } else {
        new_sample[i] = sample_[old_index];
      }
    }
    set(new_sample);
  }
  
  void zeroise() {
    std::memset(sample_, 0, sizeof(value_type) * NUM_SAMPLE);
  }
  bool is_near_zero(value_type v) const {
    for (int i=0; i<NUM_SAMPLE; ++i) {
      if (std::abs(sample_[i]) >= v) {
        return false;
      }
    }
    return true;
  }

  static Spectrum constant(value_type val) {
    Spectrum r;
    for (int i=0; i<NUM_SAMPLE; ++i) {
      r[i] = val;
    }
    return r;
  }
  static Spectrum zero() { return constant(0); }
  static Spectrum one() { return constant(1); }

  static value_type wavelength_from_index(int index)
  {
    return MIN_WAVELENGTH +
      (MAX_WAVELENGTH - MIN_WAVELENGTH) / (NUM_SAMPLE - 1) * index;
  }
  static int index_from_wavelength(value_type wavelength)
  {
    return
      (wavelength - MIN_WAVELENGTH) /
        (MAX_WAVELENGTH - MIN_WAVELENGTH) * (NUM_SAMPLE - 1);
  }
  
private:
  value_type sample_[NUM_SAMPLE];
};

#define DEF_SPECTRUM_OP(op) \
  inline \
  Spectrum& operator op=(Spectrum& a, const Spectrum b) \
  { \
    for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) { \
      a[i] op= b[i]; \
    } \
    return a; \
  } \
  inline \
  Spectrum operator op(const Spectrum& a, const Spectrum& b) \
  { \
    Spectrum r(a); \
    r op= b; \
    return r; \
  }
DEF_SPECTRUM_OP(+)
DEF_SPECTRUM_OP(*)
#undef DEF_SPECTRUM_OP

#define DEF_SPECTRUM_OP_S(op) \
  inline \
  Spectrum& operator op=(Spectrum& a, value_type b) \
  { \
    for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) { \
      a[i] op= b; \
    } \
    return a; \
  } \
  inline \
  Spectrum operator op(const Spectrum& a, value_type b) \
  { \
    Spectrum r(a); \
    r op= b; \
    return r; \
  }
DEF_SPECTRUM_OP_S(*)
DEF_SPECTRUM_OP_S(/)
#undef DEF_SPECTRUM_OP_S
inline
Spectrum operator *(value_type a, const Spectrum& b)
{
  Spectrum r(b);
  r *= a;
  return r;
}


Spectrum spectrum_from_data(const value_type *data, value_type freq);

#endif // INCLUDED_SPECTRUM_H
