#ifndef INCLUDED_CIE_H
#define INCLUDED_CIE_H


#include "color.h"
#include "spectrum.h"

XYZColor xyz_from_spectrum(const Spectrum&);
value_type intensity_from_spectrum(const Spectrum&);

inline
RGBColor rgb_from_spectrum(const Spectrum& s)
{
  return
    rgb_from_srgb(srgb_from_xyz(xyz_from_spectrum(s)));
}

#endif // ndef INCLUDED_CIE_H

