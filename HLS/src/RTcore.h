#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif


#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_channel.h>
#include <mc_scverify.h>

typedef ac_int<1, false> pbool;
typedef ac_int<2, false> uint_2;
typedef ac_int<8, false> uint_8; //mostly for rgb
typedef ac_int<10, false> uint_10; /// 11bits
typedef ac_int<11, false> uint_11; /// 11bits
typedef ac_int<11, true> int_11; /// 10 bits + sign bit? for points in world space
typedef ac_int<12, true> int_12; /// 11bits
typedef ac_int<22, false> uint_22; /// 11bits
typedef ac_fixed<23, 1, false> fp_1_22; // TO DO CHANGE THIS TO ufp for unsigned fixed point
typedef ac_fixed<25, 3, true> sfp_3_22;
typedef ac_fixed<26, 4, true> sfp_4_22;
typedef ac_fixed<19, 9, true> sfp_9_10; // leave an overflow bit + precision of 1/1000
typedef ac_fixed<21, 11, true> sfp_11_10;
typedef ac_fixed<28, 18, false> sfp_18_10; //8 shader cores means max sum is 2048 = 8 * 256
typedef ac_fixed<33, 11, true> sfp_11_22; // signed fixed point
typedef ac_fixed<34, 12, false> fp_12_22; // signed fixed point

typedef ac_int<8, false> uint_8;
typedef ac_int<64, false> uint_64;

#include "vec3.h"
#include "ray.h"
#include "Rand.h"

struct vec3_fp_11_22 { // vector point-- can be as big as the world space
  int_11 x_i;
  int_11 x_f1; // first half of fixed point
  int_11 x_f2; // second half
  int_11 y_i;
  int_11 y_f1;
  int_11 y_f2;
  int_11 z_i;
  int_11 z_f1;
  int_11 z_f2;
};

template <typename T>
struct pxl_deltas { 
  vec3<T> pixel_delta_u;
  vec3<T> pixel_delta_v;
};

struct rgb_t { // standard sdr
  int_11 r;
  int_11 g;
  int_11 b;
};

struct img_params {
  // might not even need these
//   uint_2 aspect_ratio; // 
  int_11          num_spheres;
  int_11          num_quads;
  uint_2          samp_per_pxl; // 64-256-512-1024
//   uint_2 vfov; // 20,40,60,80
  rgb_t           background;
//   vec3<int_11> lookfrom;
//   vec3<int_11> lookat;
  // derrivied values (might just pass these in)
  uint_10         image_height;   // Height of the image, computed from width and aspect ratio.
  uint_10         image_width;   // Height of the image, computed from width and aspect ratio.
  vec3<int_11>        center;         // The position of the camera (same as lookfrom).
  vec3_fp_11_22   pixel00_loc;    // Location in space of the top-left pixel.
  sfp_3_22   pixel_delta_u;  // Vector to move one pixel to the right on the image plane.
  sfp_3_22   pixel_delta_v;  // Vector to move one pixel down on the image plane.
//   cam_params(uint_2 a, uint_2 b, uint_2 c) {
//     aspect_ratio=a;
//     samp_per_pxl=b;
//   }
};

struct LoopIndices{
    uint_11 x_pxl;
    uint_11 y_pxl;
    uint_11 cur_samp;
};

struct pxl_params {
  vec3<int_11>        center;
  vec3_fp_11_22   pixel00_loc;    // Location in space of the top-left pixel.
};

#include "HitRecord.h"
#include "SphHit.h"
#include "QuadHit.h"

#define WS_MAX_X 1023
#define WS_MAX_Y 1023
#define WS_MAX_Z 1023
#define WS_MIN_X -1024
#define WS_MIN_Y -1024
#define WS_MIN_Z -1024
#define NUM_SHADER_CORES 8
#define LONGEST_DISTANCE 2896
// from point to point in the 2048x2048 box

#define LAMBERTIAN 0
#define METAL 1
#define EMISSIVE 2

// smallest value that can be represented with 22 fractional bits
#define SMALLEST 2.384185791015625e-07
#define SMALLEST_INTERVAL 0.0009765625

