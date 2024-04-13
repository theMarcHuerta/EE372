
#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "render.h"
#include <mc_scverify.h>

#include "Serializer.h"
#include "Deserializer.h"

#include "InputDoubleBuffer.h"
#include "WeightDoubleBuffer.h"
#include "SystolicArray.h"

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_channel.h>


typedef ac_int<2, false> uint_2;
typedef ac_int<8, false> uint_8; //mostly for rgb
typedef ac_int<11, true> uint_11; /// 10 bits + sign bit? for points in world space

typedef ac_int<8, false> uint_8;
typedef ac_int<64, false> uint_64;

struct ws_point {
  uint_11 x;
  uint_11 g;
  uint_11 b;
};

struct rgb_t {
  uint_8 r;
  uint_8 g;
  uint_8 b;
};

struct cam_params {
  uint_2 aspect_ratio; // 
  uint_2 samp_per_pxl; // 64-128-256-512
  uint_2 vfov; // 20,40,60,80
  rgb_t  background;
  ws_point lookfrom;
  ws_point lookat;
  
//   cam_params(uint_2 a, uint_2 b, uint_2 c) {
//     aspect_ratio=a;
//     samp_per_pxl=b;
//   }
};


#define WS_MAX_X 1023
#define WS_MAX_Y 1023
#define WS_MAX_Z 1023
#define WS_MIN_X -1024
#define WS_MIN_Y -1024
#define WS_MIN_Z -1024

#define OX0_MAX 112
#define OY0_MAX 112
#define FX_MAX 7
#define FY_MAX 7
#define IC1_MAX 512
#define OC1_MAX 512



