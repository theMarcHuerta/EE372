#pragma once

#include "ac_fixed.h"
#include "ac_int.h"

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
typedef ac_fixed<33, 11, true> sfp_11_22; // signed fixed point

typedef ac_int<8, false> uint_8;
typedef ac_int<64, false> uint_64;