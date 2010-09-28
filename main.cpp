
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdio>

#include <sdl.h>

#include "type.h"
#include "scene.h"
#include "util.h"
#include "random.h"
#include "qrandom.h"
#include "timer.h"
#include "cie.h"
#include "color.h"
#include "black_body_radiation.h"

#include "put_pixel.h"

namespace {

const int WIDTH = 512;
const int HEIGHT = 512;
const int RAY_PER_PIXEL = 5;

Spectrum *g_image;
} // end of anonymus namespace

namespace {
// スペクトル色々
#include "d65.spec"
#include "illA.spec"
#include "natriumlamp.spec"
#include "mcc.spec"
#include "bk7.spec"
#include "sk2.spec"
} // end of anonymus namespace


void render(SDL_Surface *screen)
{
  // マテリアル
  Spectrum zero_spec = Spectrum::zero();
  Spectrum D65_spec = spectrum_from_data(D65_SPEC, 1);
  Spectrum illA_spec = spectrum_from_data(ILL_A_SPEC, 5);
  Spectrum natriumlamp_spec = spectrum_from_data(NATRIUMLAMP_SPEC, 5);
  Spectrum bb3000_spec = black_body_radiation(3000);
  Spectrum bb6000_spec = black_body_radiation(6000);
  Spectrum bb9000_spec = black_body_radiation(9000);
  Spectrum perfect_spec = Spectrum::constant(1.0f);
  Material D65_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, D65_spec);
  Material illA_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, illA_spec);
  Material natriumlamp_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, natriumlamp_spec);
  Material perfect_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, perfect_spec);
  Material bb3000_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, bb3000_spec);
  Material bb6000_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, bb6000_spec);
  Material bb9000_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             1.0f, bb9000_spec);

  Spectrum mcc22_spec = spectrum_from_data(MCC_SPEC[21], 5);
  Spectrum mcc16_spec = spectrum_from_data(MCC_SPEC[15], 5);
  Spectrum mcc15_spec = spectrum_from_data(MCC_SPEC[14], 5);
  Spectrum mcc13_spec = spectrum_from_data(MCC_SPEC[12], 5);
  Spectrum mcc14_spec = spectrum_from_data(MCC_SPEC[13], 5);
  Spectrum gray_spec = Spectrum::constant(0.2f);
  Material gray_mat =
    Material(1.0f, gray_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             0.0f, zero_spec);
  Material red_mat =
    Material(1.0f, mcc15_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             0.0f, zero_spec);
  Material blue_mat =
    Material(1.0f, mcc13_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             0.0f, zero_spec);
  Material yellow_mat =
    Material(1.0f, mcc16_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             0.0f, zero_spec);
  Material green_mat =
    Material(1.0f, mcc14_spec,
             0.0f, zero_spec,
             0.0f, zero_spec, zero_spec,
             0.0f, zero_spec);
  Spectrum polish_spec = Spectrum::constant(0.9f);
  Material polish_mat =
    Material(0.2f, mcc22_spec,
             0.8f, polish_spec,
             0.0f, zero_spec, zero_spec,
             0.0f, zero_spec);
  Spectrum glass_spec = Spectrum::constant(0.9f);
  Spectrum bk7_spec = Spectrum(BK7_REFRACTIVE_INDEX);
  Spectrum sk2_spec = Spectrum(SK2_REFRACTIVE_INDEX);
  Spectrum nazo_spec = Spectrum::constant(1.0f);
  for (int i=0; i<16; ++i) {
    nazo_spec[i] = 1.4+(15-i)*1.0/15;
  }
  Material glass_mat =
    Material(0.0f, zero_spec,
             0.0f, zero_spec,
             1.0f, glass_spec, sk2_spec, //bk7_spec,
             0.0f, zero_spec);

  // シーンを準備
  SceneRoot scene;
  //  scene.set_light(Light(normalize(vector3(0, -1, 0)), 1.0));
#if 1
  // 床
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 0), vector3(0, 1, 0)), gray_mat));
#if 1
  // 天井
  scene.add_object(create_scene_object(
    Plane(vector3(0, 3, 0), vector3(0, -1, 0)),
    D65_mat));
//    gray_mat));
  // 右壁
  scene.add_object(create_scene_object(
    Plane(vector3(2.0, 0, 0), vector3(-1, 0, 0)), blue_mat));
  // 左壁
  scene.add_object(create_scene_object(
    Plane(vector3(-2.0, 0, 0), vector3(1, 0, 0)), red_mat));
  // 奥壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 5), vector3(0, 0, -1)), gray_mat));
  // 手前壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, -5), vector3(0, 0, 1)), gray_mat));
#endif
#if 1
  // 普通の球
  scene.add_object(create_scene_object(
    Sphere(vector3(0, 0.6, 0), 0.5), gray_mat));
  // ツルツルな球
  scene.add_object(create_scene_object(
    Sphere(vector3(1.1, 0.6, 0), 0.5), polish_mat));
  // 透明な球
  scene.add_object(create_scene_object(
    Sphere(vector3(0.5, 0.21, -1), 0.2), glass_mat));
#else
  // 普通の球
  scene.add_object(create_scene_object(
    Sphere(vector3(0, 0.5, 0), 0.5), red_mat));
  // ツルツルな球
  scene.add_object(create_scene_object(
    Sphere(vector3(1.1, 0.5, 0), 0.5), blue_mat));
  // 透明な球
  scene.add_object(create_scene_object(
    Sphere(vector3(0.5, 0.21, -1), 0.2), green_mat));
#endif
#endif

#if 0
  // 天井
  scene.add_object(create_scene_object(
    Plane(vector3(0, 3, 0), vector3(0, -1, 0)),
//    gray_mat));
    D65_mat));
//    perfect_mat));
//    illA_mat));
//    natriumlamp_mat));
//    bb3000_mat));
//    bb6000_mat));
//    bb9000_mat));

  // 床
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 0), vector3(0, 1, 0)), gray_mat));
  // 右壁
  scene.add_object(create_scene_object(
    Plane(vector3(2.0, 0, 0), vector3(-1, 0, 0)), gray_mat));
  // 左壁
  scene.add_object(create_scene_object(
    Plane(vector3(-2.0, 0, 0), vector3(1, 0, 0)), gray_mat));
  // 奥壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 5), vector3(0, 0, -1)), gray_mat));
  // 手前壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, -5), vector3(0, 0, 1)), gray_mat));

  for (int i=0; i<24; ++i) {
    Spectrum spec = spectrum_from_data(MCC_SPEC[i], 5);
    Material mat =
      Material(1.0f, spec,
               0.0f, zero_spec,
               0.0f, zero_spec, 0.0f,
               0.0f, zero_spec);
    value_type r = 0.2f;
    value_type x = (i % 6) * 2 * r - 5 * r;
    value_type z = (i / 6) * -2 * r + 4 * r;
    value_type y = r;
    scene.add_object(
      create_scene_object(Sphere(vector3(x, y, z), r), mat));
  }
#endif

#if 0
  // 床
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 0), vector3(0, 1, 0)), gray_mat));
  // 天井
  scene.add_object(create_scene_object(
    Plane(vector3(0, 3, 0), vector3(0, -1, 0)),
//    bb3000_mat));
//    D65_mat));
    gray_mat));
//    natriumlamp_mat));
  // 右壁
  scene.add_object(create_scene_object(
    Plane(vector3(2.0, 0, 0), vector3(-1, 0, 0)), red_mat));
  // 左壁
  scene.add_object(create_scene_object(
    Plane(vector3(-2.0, 0, 0), vector3(1, 0, 0)), blue_mat));
  // 奥壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 5), vector3(0, 0, -1)), gray_mat));
  // 手前壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, -5), vector3(0, 0, 1)), gray_mat));
  // 透明な球
  scene.add_object(create_scene_object(
    Sphere(vector3(0.0, 0.8, -0.2), 0.8), glass_mat));
#endif

#if 0
  // 床
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 0), vector3(0, 1, 0)), gray_mat));
  // 天井
  scene.add_object(create_scene_object(
    Plane(vector3(0, 1.5, 0), vector3(0, -1, 0)),
//    bb3000_mat));
//    D65_mat));
    gray_mat));
//    natriumlamp_mat));
  // 右壁
  scene.add_object(create_scene_object(
    Plane(vector3(2.0, 0, 0), vector3(-1, 0, 0)),
    gray_mat));
//    red_mat));
//    D65_mat));
  // 左壁
  scene.add_object(create_scene_object(
    Plane(vector3(-2.0, 0, 0), vector3(1, 0, 0)),
//    D65_mat));
//    blue_mat));
    gray_mat));
  // 奥壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, 5), vector3(0, 0, -1)),
//    yellow_mat));
    gray_mat));
  // 手前壁
  scene.add_object(create_scene_object(
    Plane(vector3(0, 0, -5), vector3(0, 0, 1)), gray_mat));
  // 透明な球
  scene.add_object(create_scene_object(
    Sphere(vector3(0.0, 0.7, 0.0), 0.5), glass_mat));
#endif
  
  if (0)
  {// 光源
    vector3 p[] = {
      vector3(-0.5, 1.49, -0.5),
      vector3(0.5, 1.49, -0.5),
      vector3(0.5, 1.49, 0.5),
      vector3(-0.5, 1.49, 0.5),
    };
    vector3 n(0, -1, 0);
    Material mat = D65_mat;
    scene.add_object(create_scene_object(
      Triangle(p[0], n, p[1], n, p[2], n), mat));
    scene.add_object(create_scene_object(
      Triangle(p[0], n, p[2], n, p[3], n), mat));
  }
  if (0)
  {// 光源
    vector3 p[] = {
      vector3(-0.6, 0.5, -0.5),
      vector3(-0.6, 0.6, -0.5),
      vector3(-0.6, 0.6, 0.5),
      vector3(-0.6, 0.5, 0.5),
    };
    vector3 n(1, 0, 0);
    Material mat = D65_mat;
    scene.add_object(create_scene_object(
      Triangle(p[0], n, p[1], n, p[2], n), mat));
    scene.add_object(create_scene_object(
      Triangle(p[0], n, p[2], n, p[3], n), mat));
  }
  if (0)
  { // プリズム
    Material prism_mat = //yellow_mat;
      Material(0.0f, zero_spec,
               0.0f, zero_spec,
               1.0f, glass_spec, sk2_spec, //bk7_spec,
               0.0f, zero_spec);
#if 0
    vector3 p[] = {
      vector3(-0.5, 0.2, -0.5-1.0),
      vector3(-0.5, 0.2, 0.5-1.0),
      vector3(-0.5, 0.7, 0.0-1.0),
      vector3(0.5, 0.2, -0.5-1.0),
      vector3(0.5, 0.2, 0.5-1.0),
      vector3(0.5, 0.7, 0.0-1.0),
    };
#else
    vector3 p[] = {
      vector3(-0.5, 0.2, -0.5),
      vector3( 0.5, 0.2, -0.5),
      vector3( 0.0, 0.7, -0.5),
      vector3(-0.5, 0.2,  0.5),
      vector3( 0.5, 0.2,  0.5),
      vector3( 0.0, 0.7,  0.5),
    };
#endif
    struct AddTriangle{
      AddTriangle(SceneRoot& scene, const Material& mat)
        : scene(scene), mat(mat) {}
      void operator()(const vector3& p0, const vector3& p1, const vector3& p2)
      {
        vector3 n = cross(normalize(p1 - p0), normalize(p2 - p0));
        scene.add_object(create_scene_object(
          Triangle(p0, n, p1, n, p2, n), mat));
      }
      SceneRoot& scene;
      const Material& mat;
    } add_triangle(scene, prism_mat);
    add_triangle(p[0], p[1], p[2]);
    add_triangle(p[3], p[4], p[5]);

    add_triangle(p[2], p[0], p[5]);
    add_triangle(p[0], p[3], p[5]);

    add_triangle(p[1], p[2], p[5]);
    add_triangle(p[5], p[4], p[1]);

    add_triangle(p[1], p[3], p[4]);
    add_triangle(p[1], p[0], p[3]);
  }

#if 0
  // 球光源
  scene.add_object(create_scene_object(
    Sphere(vector3(0, 2.0, 0.0), 0.5),
//    perfect_mat));
    D65_mat));
//    illA_mat));
//    natriumlamp_mat));
#endif

  vector3 camera_pos(0, 1.0, -4);
//  vector3 camera_pos(0, 2.0, -3);
  vector3 camera_dir = normalize(-camera_pos);
  value_type screen_width = .08;
  value_type screen_height = .08;
  value_type near = .1;
  int ray_per_pixel = RAY_PER_PIXEL;
  value_type subpixel_width = screen_width / (WIDTH * ray_per_pixel);
  value_type subpixel_height = screen_height / (HEIGHT * ray_per_pixel);
  g_image = new Spectrum[HEIGHT*WIDTH];
  int vdc_index = 0;
  for (int y=0; y<HEIGHT; ++y) {
    for (int x=0; x<WIDTH; ++x) {
      Spectrum spectrum = Spectrum::zero();
      Spectrum tmp;
      vector3 org = camera_pos + camera_dir * near;
      org.x += screen_width * (x / static_cast<value_type>(WIDTH) - 0.5);
      org.y -= screen_height * (y / static_cast<value_type>(HEIGHT) - 0.5);
      for (int i=0; i<ray_per_pixel; ++i) {
        for (int j=0; j<ray_per_pixel; ++j) {
          vector3 s = org;
          s.x += (i + vdC(vdc_index++, 2)) * subpixel_width;
          s.y -= (j + vdC(vdc_index++, 2)) * subpixel_height;
          Ray ray(s, normalize(s - camera_pos));
          scene.traverse_ray(ray, &tmp);
          spectrum += tmp;
        }
      }
      spectrum /= ray_per_pixel * ray_per_pixel;
      g_image[y*WIDTH+x] = spectrum;
      RGBColor rgb = rgb_from_spectrum(spectrum);
      put_pixel(screen, x, y, rgb.r*0xff, rgb.g*0xff, rgb.b*0xff);
    }
    if ((y % 10) == 0) {
      SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
  }
  SDL_UpdateRect(screen, 0, 0, 0, 0);

  {
    // トーンマッピング
    value_type total = 0;
    for (int y=0; y<HEIGHT; ++y) {
      for (int x=0; x<WIDTH; ++x) {
        value_type intensity = intensity_from_spectrum(g_image[y*WIDTH+x]);
        total += intensity;
      }
    }
    value_type KEY_VALUE = 0.18f;
    value_type scale = KEY_VALUE * HEIGHT * WIDTH / total;
    for (int y=0; y<HEIGHT; ++y) {
      for (int x=0; x<WIDTH; ++x) {
        const Spectrum& s = g_image[y*WIDTH+x];
        Spectrum st = s * scale;
        RGBColor rgb = rgb_from_spectrum(st);
        put_pixel(screen,
                  x, y,
                  rgb.r*0xff,
                  rgb.g*0xff,
                  rgb.b*0xff);
      }
    }
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }
}

void development(SDL_Surface *surface, value_type explosure)
{
  for (int y=0; y<HEIGHT; ++y) {
    for (int x=0; x<WIDTH; ++x) {
      const Spectrum& s = g_image[y*WIDTH+x];
      Spectrum st = s * explosure;
      RGBColor rgb = rgb_from_spectrum(st);
      put_pixel(surface,
                x, y,
                rgb.r*0xff,
                rgb.g*0xff,
                rgb.b*0xff);
    }
  }
  SDL_UpdateRect(surface, 0, 0, 0, 0);
}

void save_bmp(SDL_Surface *surface, unsigned elapsed)
{
  std::time_t t = std::time(0);
  const std::tm *ltm = std::localtime(&t);
  char filename[256];
  _snprintf(filename,
            sizeof(filename),
            "result/%d%02d%02d%02d%02d%02d_%u.bmp",
            ltm->tm_year + 1900,
            ltm->tm_mon + 1,
            ltm->tm_mday,
            ltm->tm_hour,
            ltm->tm_min,
            ltm->tm_sec,
            elapsed);
  filename[sizeof(filename)-1] = '\0';
  SDL_SaveBMP(surface, filename);
}

int main(int argc, char **argv)
{
  SDL_Surface *screen;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL の 初期化に失敗: " << SDL_GetError() << std::endl;
    return 1;
  }

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);
  if (!screen) {
    std::cerr << "ビデオモードの設定に失敗: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  Timer timer;
  render(screen);
  unsigned elapsed = timer.elapsed_sec();
  char buf[32];
  _snprintf(buf, sizeof(buf), "elapsed %d sec", elapsed);
  SDL_WM_SetCaption(buf, 0);
  save_bmp(screen, elapsed);

  value_type explosure = 1.0f;

  SDL_Event e;
  bool loop = true;
  while (loop) {
    if (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE:
        case SDLK_q:
          loop = false;
          break;
        case SDLK_r:
          render(screen);
          break;
        case SDLK_UP:
          explosure *= 2.0f;
          development(screen, explosure);
          break;
        case SDLK_DOWN:
          explosure *= 0.5f;
          development(screen, explosure);
          break;
        case SDLK_s:
          save_bmp(screen, elapsed);
          break;
        }
        break;
      case SDL_QUIT:
        loop = false;
        break;
      }
    }
    SDL_Delay(1);
  }
  
  SDL_Quit();
  
  return 0;
}
