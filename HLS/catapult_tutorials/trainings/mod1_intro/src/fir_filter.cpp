//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "fir_filter.h"

void fir_filter (const double i_sample,
                 double b[],
                 double &y)
{
  // previous input history, remember across calls
  static double x[TAP_COUNT];
  SHIFT_LOOP:for (int n=TAP_COUNT-1; n>0; n--) {
    x[n] = x[n-1];
  }
  x[0] = i_sample;

  double sum = 0;
  MAC_LOOP:for (unsigned n=0; n<TAP_COUNT; n++) {
    sum += x[n] * b[n];
  }
  y = sum;
}
