//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

/*
 * low-pass filter
 */

#include "top.h"

#pragma hls_design block
bool fir_filter(
  ac_channel<X_TYPE>       &x_chan,
  ac_channel<Y_TYPE>       &y_chan,
  ac_channel<MPU_STREAM_TYPE> &mpu_in,
  ac_channel<COEFF_TYPE>     &mpu_out)
{
  bool busy = false;
  static COEFF_TYPE  b[TAP_COUNT];

#ifndef __SYNTHESIS__
  while (mpu_in.available(1))
#endif
  {
    busy = true;
    MPU_STREAM_TYPE   mpu_request = mpu_in.read();
#ifndef __SYNTHESIS__
    assert(mpu_request.addr < TAP_COUNT);
#endif
    if (mpu_request.addr.to_uint() < TAP_COUNT) {
      if (mpu_request.read) {
        mpu_out.write(b[mpu_request.addr.to_uint()]);
      } else {
        //cout << __FILE__ << "," << __LINE__ << ":writing " << mpu_request.b << " to address " << mpu_request.addr << endl;
        b[mpu_request.addr] = mpu_request.data;
      }
    }
  } // mpu_in.available()

#ifndef __SYNTHESIS__
  while (x_chan.available(1))
#endif
  {
    busy = true;   // let top-level know we have done some processing
    static X_TYPE  x[TAP_COUNT];  // previous input history, remember across calls

#pragma hls_unroll yes
    SHIFT_LOOP:for (int n=TAP_COUNT-1; n>0; n--) {
      x[n] = x[n-1];
    }
    x[0] = x_chan.read();

    // summation size derived type:
    typedef X_TYPE::rt_T<COEFF_TYPE>::mult         PROD_TYPE;
    typedef PROD_TYPE::rt_unary::set<TAP_COUNT>::sum  SUM_TYPE;

    SUM_TYPE  sum = 0;

    // Coefficients are always symmetric around center;
    //   trade multipliers for adders:
    MAC_LOOP:for (
      unsigned up=0, down=TAP_COUNT-1;
      up<TAP_COUNT/2;
      up++, down--
    ) {
      sum = sum + (x[up] + x[down]) * b[up];
#ifndef __SYNTHESIS__
      assert(b[up]==b[down]);  // confirm symmetric coefficient assumption
#endif
    }

    // if there are an odd number of taps, add in the center addend:
    if (TAP_COUNT%2 != 0) {
      const unsigned CENTER_TAP = TAP_COUNT/2;
      sum = sum + x[CENTER_TAP] * b[CENTER_TAP];
    }
    Y_TYPE round_sum = sum;  // round and sat to Y_TYPE
    y_chan.write(round_sum);
  } // while
  return (busy);
}
