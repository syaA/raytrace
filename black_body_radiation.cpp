
#include "black_body_radiation.h"

#include <cmath>


Spectrum black_body_radiation(value_type temp)
{
  // 2hc^2
  const double coef0 = 1.19104259e-16;
  // hc/kT
  const double coef1 = 0.0143877506 / temp;
  
  Spectrum s;
  double dsample =
    (Spectrum::MAX_WAVELENGTH - Spectrum::MIN_WAVELENGTH) /
      (Spectrum::NUM_SAMPLE - 1);
  double max = 0.0;
  for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) {
    double wavelength =
      (Spectrum::MIN_WAVELENGTH + dsample * i) * 1e-9;
    s[i] = coef0 /
      (std::pow(wavelength, 5) * (std::exp(coef1 / wavelength) - 1));
    if (s[i] > max) {
      max = s[i];
    }
  }
  return s / max; // normalized
}
