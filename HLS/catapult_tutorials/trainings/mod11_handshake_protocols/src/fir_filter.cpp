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

using namespace std;
#include "fir_filter.h"
#pragma hls_design top
void fir_filter(ac_channel<X_TYPE> &i_sample, COEFF_TYPE b[TAP_COUNT], ac_channel<Y_TYPE> &y)
{
  static X_TYPE  x[TAP_COUNT];  // previous input history, remember across calls
#ifndef __SYNTHESIS__
  while (i_sample.available(1))
#endif
  {
#pragma hls_unroll yes
    SHIFT_LOOP:for (int n=TAP_COUNT-1; n>0; n--) {
      x[n] = x[n-1];
    }
    x[0] = i_sample.read();

    // summation size derived type:
    typedef X_TYPE::rt_T<COEFF_TYPE>::mult         PROD_TYPE;
    typedef PROD_TYPE::rt_unary::set<TAP_COUNT>::sum  SUM_TYPE;                         // @suppress("Type cannot be resolved")

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

    Y_TYPE y_sum = sum;    // round and sat to Y_TYPE
    y.write(y_sum);
  }
}
