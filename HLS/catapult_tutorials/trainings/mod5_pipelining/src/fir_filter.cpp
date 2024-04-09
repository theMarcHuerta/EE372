//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "fir_filter.h"

#pragma hls_design top
void fir_filter(const X_TYPE i_sample,
                COEFF_TYPE b[TAP_COUNT],
                Y_TYPE &y)
{
  static X_TYPE  x[TAP_COUNT];  // previous input history, remember across calls
  SHIFT_LOOP:for (int n=TAP_COUNT-1; n>0; n--) {
    x[n] = x[n-1];
  }
  x[0] = i_sample;

  // summation size derived type:
  typedef X_TYPE::rt_T<COEFF_TYPE>::mult         PROD_TYPE;
  typedef PROD_TYPE::rt_unary::set<TAP_COUNT>::sum  SUM_TYPE;

  SUM_TYPE  sum = 0;

  MAC_LOOP:for (unsigned i=0; i<TAP_COUNT; i++) { // tap_count = 127
    sum = sum + x[i] * b[i];
  }

  y = sum;  // round and saturate to output format
}

