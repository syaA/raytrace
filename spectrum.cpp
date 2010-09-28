
#include "spectrum.h"

#include <cmath>

#include "util.h"


const value_type Spectrum::MIN_WAVELENGTH = 380; // nm
const value_type Spectrum::MAX_WAVELENGTH = 780; // nm

Spectrum spectrum_from_data(const value_type *data, value_type freq)
{
  Spectrum s;
  value_type dsample =
    (Spectrum::MAX_WAVELENGTH - Spectrum::MIN_WAVELENGTH)
      / (Spectrum::NUM_SAMPLE - 1) / freq;
  for (int i=0; i<Spectrum::NUM_SAMPLE; ++i) {
    value_type wavelength = dsample * i;
    int wl_ceil = static_cast<int>(std::ceil(wavelength));
    int wl_floor = static_cast<int>(std::floor(wavelength));
    
    s[i] = lerp(data[wl_floor], data[wl_ceil], wl_ceil - wavelength);
  }
  return s;

}
