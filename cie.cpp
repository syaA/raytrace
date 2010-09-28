
#include <cmath>

#include "type.h"
#include "cie.h"

namespace {
#include "cie_sco.dat"
#include "photopic_lef.dat"
} // end of anonymus namespace

XYZColor xyz_from_spectrum(const Spectrum& s)
{
  // スペクトル→XYZ
  XYZColor xyz = { 0, 0, 0};
  for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) {
    int wavelength =
      static_cast<int>(
        (Spectrum::MAX_WAVELENGTH - Spectrum::MIN_WAVELENGTH) /
        (Spectrum::NUM_SAMPLE - 1) * i);
    xyz.x += s[i] * CIE_STANDARD_COLORMETRIC_OBSERVER[wavelength].x;
    xyz.y += s[i] * CIE_STANDARD_COLORMETRIC_OBSERVER[wavelength].y;
    xyz.z += s[i] * CIE_STANDARD_COLORMETRIC_OBSERVER[wavelength].z;
  }
  return xyz;
}

value_type intensity_from_spectrum(const Spectrum& s)
{
  value_type intensity = 0;
  for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) {
    int wavelength =
      static_cast<int>(
        (Spectrum::MAX_WAVELENGTH - Spectrum::MIN_WAVELENGTH) /
        (Spectrum::NUM_SAMPLE - 1) * i);
    intensity += s[i] * PHOTOPIC_LUMINOUS_EFFICIENCY_FUNCTION[wavelength];
  }
  return intensity;
}
