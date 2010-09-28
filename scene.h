#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <vector>
#include <limits>

#include "vec.h"
#include "ray.h"
#include "primitive.h"
#include "material.h"
#include "random.h"
#include "spectrum.h"


class SceneObjectBase
{
public:
  virtual ~SceneObjectBase() {}

  virtual value_type intersect_ray(const Ray& ray) const = 0;
  virtual bool reflect_ray(Ray& ray, value_type t) const = 0;
};

template<class ShapeT>
class SceneObject : public SceneObjectBase
{
public:
  SceneObject(const ShapeT& shape, const Material& mat)
    : shape_(shape), mat_(mat)
  {}

  const ShapeT& shape() const { return shape_ };
  const Material& material() const { return mat_; }

  virtual value_type intersect_ray(const Ray& ray) const
  {
    return intersect(ray, shape_);
  }

  virtual bool reflect_ray(Ray& ray, value_type t) const
  {
    vector3 pos = ray.position(t);
    return mat_.traverse(ray, pos, shape_.normal(pos), t);
  }

private:
  ShapeT shape_;
  Material mat_;
};

template<class ShapeT>
SceneObject<ShapeT> *create_scene_object(
  const ShapeT& shape, const Material& mat)
{
  return new SceneObject<ShapeT>(shape, mat);
}

class Light
{
public:
  Light()
    : dir_(0, -1, 0), effective_(false)
  {}
  Light(const vector3& dir, const Spectrum& spectrum)
    : dir_(dir), effective_(true), spectrum_(spectrum)
  {}

  const vector3& direction() const { return dir_; }
  bool effective() const { return effective_; }
  const Spectrum& spectrum() const { return spectrum_; }

private:
  vector3 dir_;
  bool effective_;
  Spectrum spectrum_;
};

struct IntersectInfo
{
  IntersectInfo(value_type t, const SceneObjectBase *obj)
    : t(t), obj(obj)
  {}
  value_type t;
  const SceneObjectBase *obj;
};

class SceneRoot
{
public:
  typedef std::vector<const SceneObjectBase*> scn_objs;

public:
  SceneRoot()
    : max_ref_count_(8), cutoff_decay_(1e-8f)
  {}
  ~SceneRoot()
  {
    for (scn_objs::iterator it = scn_objs_.begin(),
                            ite = scn_objs_.end();
         it != ite;
         ++it)
    {
      delete *it;
    }
  }
  IntersectInfo intersect_ray(const Ray& ray)
  {
    // レイが当たるオブジェクトを探す
    value_type min_t = std::numeric_limits<value_type>::max();
    const SceneObjectBase *obj = 0;
    for (scn_objs::iterator it = scn_objs_.begin(),
         ite = scn_objs_.end();
         it != ite;
         ++it)
    {
      value_type t = (*it)->intersect_ray(ray);
      if (t > 0 && t < min_t) {
        min_t = t;
        obj = *it;
      }
    }
    return IntersectInfo(min_t, obj);
  }
  void traverse_ray(Ray ray, Spectrum *out)
  {
    int ref_count = 0;
    for (;;) {
      // 反射物を探す
      IntersectInfo isect = intersect_ray(ray);
      // 反射させる
      if (isect.obj) {
        ++ref_count;
        if (isect.obj->reflect_ray(ray, isect.t)) {
          if (ref_count >= max_ref_count_ ||
              ray.decay().is_near_zero(cutoff_decay_)) {
            // 反復打ち切り
            break;
          }
        } else {
          // 反復打ち切り
          break;
        }
      } else {
        // 何にも当たらなかった
        break;
      }
    }

    if (ref_count > 0) {
      finalize_ray(ray, out);
    } else {
      out->zeroise();
    }
  }
  void finalize_ray(const Ray& ray, Spectrum *out)
  {
    if (ray.achive_emissive()) {
      // 光源に当たった
      *out = ray.spectrum();
    } else {
      // 光源に当たらなかった
      if (light_.effective()) {
        // グローバルなライトが存在する
        value_type light_rate = dot(light_.direction(), -ray.direction());
        if (light_rate <= 0) {
          out->zeroise();
        } else {
          *out = ray.decay() * light_rate * light_.spectrum();
        }
      } else {
        // グローバルなライトは存在しない
        out->zeroise();
      }
    }
  }

  size_t add_object(const SceneObjectBase *obj)
  {
    scn_objs_.push_back(obj);
    return scn_objs_.size();
  }

  void set_light(const Light& light) { light_ = light; }
  const Light& light() const { return light_; }

  void set_traverse_parameter(int max_ref_count, value_type cutoff_decay)
  {
    max_ref_count_ = max_ref_count;
    cutoff_decay_ = cutoff_decay_;
  }
private:
  scn_objs scn_objs_;
  Light light_;
  int max_ref_count_;
  value_type cutoff_decay_;
};

#endif // def INCLUDED_SCENE_H
