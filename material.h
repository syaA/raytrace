#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "type.h"
#include "vec.h"
#include "random.h"
#include "spectrum.h"

class Material
{
public:
  Material(value_type diffuse_odds,
           const Spectrum& diffuse_spectrum,
           value_type reflect_odds,
           const Spectrum& reflect_spectrum,
           value_type refract_odds,
           const Spectrum& refract_spectrum,
           const Spectrum& refract_index,
           value_type  emissive_odds,
           const Spectrum& emissive_spectrum)
    : dif_odds_(diffuse_odds),
      dif_spectrum_(diffuse_spectrum),
      spe_odds_(reflect_odds + dif_odds_),
      spe_spectrum_(reflect_spectrum),
      ref_odds_(refract_odds + spe_odds_),
      ref_spectrum_(refract_spectrum),
      ref_index_(refract_index),
      emi_odds_(emissive_odds + ref_odds_),
      emi_spectrum_(emissive_spectrum)
  {
  }
  
  bool traverse(
    Ray& ray, const vector3& pos, const vector3& nml, value_type t) const
  {
    // 反射の時のオフセット
    value_type ofs = 1e-5;
    // 拡散反射/鏡面反射を選択
    value_type r = rand_01();
    if (r <= dif_odds_) {
      // 拡散反射
      // 半球上でサンプリング
      vector3 out = rand_on_sphere(); 
      if (dot(out, nml) < 0) {
        out = -out;
      }
      ray.reflect(pos+out*ofs, out, dif_spectrum_);
      return true;
    } else if (r <= spe_odds_) {
      // 鏡面反射
      vector3 out = reflect(ray.direction(), nml);
      ray.reflect(pos+out*ofs, out, spe_spectrum_);
      return true;
    } else if (r <= ref_odds_) {
      // 屈折
      value_type vn = dot(ray.direction(), nml);
#if 0
      // 屈折させる波長をサンプリング
      int wave_index = rand_num(Spectrum::NUM_SAMPLE);
      // 既に屈折して単色光になっているかもしれない。
      wave_index = ray.pick_wavelength(wave_index);
      value_type ref_index = ref_index_[wave_index];
      if (vn < 0) {
        // 物体内へ入ってきた
        value_type rel_index = 1 / ref_index;
        value_type D = 1 - rel_index * rel_index * (1 - vn * vn);
        if (D < 0) {
          // 全反射
          vector3 out = reflect(ray.direction(), nml);
          ray.reflect(pos+out*ofs, out, spe_spectrum_);
        } else {
          vector3 out = rel_index * (ray.direction() - vn * nml)
            - std::sqrt(D) * nml;
          ray.refract(pos+out*ofs, out, ref_spectrum_, 1/rel_index);
        }
      } else {
        // 物体から出て行った
        vn = dot(ray.direction(), -nml);
        value_type rel_index = ref_index / 1;
        value_type D = 1 - rel_index * rel_index * (1 - vn * vn);
        if (D < 0) {
          // 全反射
          vector3 out = ::reflect(ray.direction(), -nml);
          ray.reflect(pos+out*ofs, out, spe_spectrum_);
        } else {
          vector3 out = rel_index * (ray.direction() + vn * nml)
            + std::sqrt(D) * nml;
          ray.refract(pos+out*ofs, out, ref_spectrum_, 1/rel_index);
        }
      }
#else
      if (vn < 0) {
        // 物体内へ入ってきた
        value_type rel_index = 1 / ref_index_[0];
        value_type D = 1 - rel_index * rel_index * (1 - vn * vn);
        if (D < 0) {
          // 全反射
          vector3 out = reflect(ray.direction(), nml);
          ray.reflect(pos+out*ofs, out, spe_spectrum_);
        } else {
          vector3 out = rel_index * (ray.direction() - vn * nml)
            - std::sqrt(D) * nml;
          ray.refract(pos+out*ofs, out, ref_spectrum_, rel_index);
        }
      } else {
        // 物体から出て行った
        vn = dot(ray.direction(), -nml);
        value_type rel_index = ref_index_[0] / 1;
        value_type D = 1 - rel_index * rel_index * (1 - vn * vn);
        if (D < 0) {
          // 全反射
          vector3 out = ::reflect(ray.direction(), -nml);
          ray.reflect(pos+out*ofs, out, spe_spectrum_);
        } else {
          vector3 out = rel_index * (ray.direction() + vn * nml)
            + std::sqrt(D) * nml;
          ray.refract(pos+out*ofs, out, ref_spectrum_, rel_index);
        }
      }
#endif
      return true;
    } else if (r <= emi_odds_) {
      // 発光している
      ray.achieve_emissive(emi_spectrum_);
      return false;
    } else {
      // 吸収
      ray.absorb();
      return false;
    }
  }
  
private:
  value_type dif_odds_;
  Spectrum dif_spectrum_;
  value_type spe_odds_;
  Spectrum spe_spectrum_;
  value_type ref_odds_;
  Spectrum ref_spectrum_;
  Spectrum ref_index_;
  value_type emi_odds_;
  Spectrum emi_spectrum_;
};

#endif // ndef INCLUDED_MATERIAL_H
