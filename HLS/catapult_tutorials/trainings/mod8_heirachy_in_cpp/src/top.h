//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef TOP_H_
#define TOP_H_

#include <ac_fixed.h>
#include <ac_int.h>
#include <ac_channel.h>
#include <assert.h>
#include <iostream>
using namespace std;

const unsigned FILTER_ORDER   = 126;
const unsigned TAP_COUNT      = FILTER_ORDER+1; // there are N+1 coefficients for an N-order filter
const unsigned RATE_BITS      = 3;
const unsigned MAX_RATE       = 1<<RATE_BITS;
const unsigned MPU_ADDR_WIDTH = ac::log2_ceil<TAP_COUNT>::val;

typedef ac_fixed<3,1,true>                   X_TYPE;
typedef ac_fixed<10,-2,true>                 COEFF_TYPE;
typedef ac_fixed<9,1,true,AC_RND,AC_SAT_SYM> Y_TYPE;
typedef ac_int<3,false>                      RATE_TYPE;
typedef struct {
  ac_int<MPU_ADDR_WIDTH,false>  addr;
  bool                    read;
  COEFF_TYPE                 data;
} MPU_STREAM_TYPE;

bool fir_filter   (
  ac_channel<X_TYPE> &,
  ac_channel<Y_TYPE> &,
  ac_channel<MPU_STREAM_TYPE> &,
  ac_channel<COEFF_TYPE> &
);
bool decimator (
  ac_channel<Y_TYPE> &,
  const RATE_TYPE,
  ac_channel<Y_TYPE> &
);
void top(
  ac_channel<X_TYPE> &,
  const RATE_TYPE,
  ac_channel<Y_TYPE> &,
  ac_channel<MPU_STREAM_TYPE> &,
  ac_channel<COEFF_TYPE>     &
);

#endif /* TOP_H_ */

