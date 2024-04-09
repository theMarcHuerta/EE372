//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "fir_filter.h"

void fir_filter (const X_TYPE i_sample,
                 COEFF_TYPE b[TAP_COUNT],
                 Y_TYPE &y)
{
  // previous input history, remember across calls
  static X_TYPE x[TAP_COUNT];
  SHIFT_LOOP:for (int n=TAP_COUNT-1; n>0; n--) {
    x[n] = x[n-1];
  }
  x[0] = i_sample;

  // intermediate variable data types:
  typedef X_TYPE::rt_T<COEFF_TYPE>::mult           PROD_TYPE;
  typedef PROD_TYPE::rt_unary::set<TAP_COUNT>::sum SUM_TYPE;

  // full-accuracy fixed-point for summation
  SUM_TYPE sum = 0;
  MAC_LOOP:for (unsigned n=0; n<TAP_COUNT; n++) {
    sum += x[n] * b[n];
  }
  // round & saturate according to Y_TYPE
  y = sum;
}
