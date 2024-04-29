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
#define METAL_MIN 1
#define METAL_MAX 6
#define EMISSIVE 7
#define MAX_SPHERES_IN_BUFFER 80
#define MAX_QUADS_IN_BUFFER 80
#define MAX_SAMPS_PER_PIXEL 1024
#define MAX_IMAGE_WIDTH 2048
#define MAX_IMAGE_HEIGHT 2048

// smallest value that can be represented with 22 fractional bits
#define SMALLEST 2.384185791015625e-07
#define SMALLEST_INTERVAL 0.0009765625

typedef ac_int<1, false> pbool;
typedef ac_int<2, false> uint_2;
typedef ac_int<3, false> uint_3;
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


struct steamed_in_quad_hittable {
    vec3<int_11> corner_pt; // for quads its corner
    vec3<int_11> u; // defining u component 
    vec3<int_11> v; // defining v componenet
    uint_3 mat_type; // allows for 4 possible materials, light, lambertian, metallic/specular, diaelectric??
    pbool is_invis;
    vec3<int_11> normal; // cross of u and v
    vec3<int_11> w; // dot of u and v
    int_12 d_plane;
    rgb_in quad_color;
};

// create an additional templatated version for the struct to allow for arithmetic in function, without screwing
// up I/O
struct quad_hittable {
    vec3<ac_fixed<12, 11, true>> corner_pt;
    vec3<ac_fixed<12, 11, true>> u; // defining u component 
    vec3<ac_fixed<12, 11, true>> v; // defining v componenet
    uint_3 mat_type; // allows for 4 possible materials, light, lambertian, metallic/specular, diaelectric??
    pbool is_invis;
    vec3<ac_fixed<26, 2, true>> normal; // cross of u and v, 2_24
    vec3<ac_fixed<25, 1, true>> w; // dot of u and v, 1_24
    ac_fixed<31, 11, true> d_plane; // 11_20
    rgb_in quad_color;
};

struct HitRecord {
    vec3<ac_fixed<21, 11, true>> hit_loc;
    vec3<ac_fixed<26, 2, true>> normal;
    bool front_face;
    // WE DONT EVEN NEED T BEYOND WORLD HIT SO MAYBE REMOVE IT OR CREATE NEW STRUCT OUT OF WORLD HIT
    // ac_fixed<43, 13, true> t; // how far along the hit occured, 11_30
    uint_3 mat;
    rgb_in color;

};

struct ray {
  vec3<ac_fixed<21, 11, true>> orig; 
  vec3<ac_fixed<34, 11, true>> dir;
  bool camera_ray;
};

template <typename T>
struct pxl_deltas { 
  vec3<T> pixel_delta_u;
  vec3<T> pixel_delta_v;
};

struct rgb_in {
  fp_1_22 r;
  fp_1_22 g;
  fp_1_22 b;
};

struct rgb_out {
  uint_8 r;
  uint_8 g;
  uint_8 b;
};

struct img_params {
  int_11          num_spheres;
  int_11          num_quads;
  uint_2          samp_per_pxl; // 32-64-256-1024
  rgb_in           background;
  uint_10         image_height;   // Height of the image, computed from width and aspect ratio.
  uint_10         image_width;   // Height of the image, computed from width and aspect ratio.
  vec3<int_11>        center;         // The position of the camera (same as lookfrom).
  vec3<sfp_11_22>   pixel00_loc;    // Location in space of the top-left pixel.
  vec3<sfp_3_22>   pixel_delta_u;  // Vector to move one pixel to the right on the image plane.
  vec3<sfp_3_22>   pixel_delta_v;  // Vector to move one pixel down on the image plane.
};

struct buffer_params {
  int_11          num_spheres;
  int_11          num_quads;
  uint_2          samp_per_pxl; // 32-64-256-1024
  uint_10         image_height;   // Height of the image, computed from width and aspect ratio.
  uint_10         image_width;   // Height of the image, computed from width and aspect ratio.
};


struct buffer_obj_count {
  int_11          num_spheres;
  int_11          num_quads;
  rgb_in          background;
 };

struct LoopIndices{
  uint_11 x_pxl;
  uint_11 y_pxl;
  uint_11 cur_samp;
};

template<typename T, int N>
struct chanStruct{
  T data[N];
};

struct pxl_params {
  vec3<int_11>        center;
  vec3<sfp_11_22>   pixel00_loc;    // Location in space of the top-left pixel.
};

