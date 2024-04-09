//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "top.h"

// Include header to support verification
#include <mc_scverify.h>

#pragma hls_design top
void CCS_BLOCK(top)(
  ac_channel<X_TYPE>          &x,
  const RATE_TYPE             M,
  ac_channel<Y_TYPE>          &y,
  ac_channel<MPU_STREAM_TYPE> &mpu_in,
  ac_channel<COEFF_TYPE>      &mpu_out
)
{
#ifndef __SYNTHESIS__
  bool  busy = true;
  while (busy)
#endif
  {
    bool fir_busy = false;
    bool decimator_busy = false;

    static ac_channel<Y_TYPE>  filtered_stream;

    fir_busy       = fir_filter(x, filtered_stream, mpu_in, mpu_out);
    decimator_busy = decimator(filtered_stream, M, y);

#ifndef __SYNTHESIS__
    busy = fir_busy || decimator_busy;
#endif
  } // busy
}

