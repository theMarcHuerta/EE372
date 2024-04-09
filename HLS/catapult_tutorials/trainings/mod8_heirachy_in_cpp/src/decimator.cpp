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

#pragma hls_design block
bool CCS_BLOCK(decimator) (
  ac_channel<Y_TYPE> &in_chan,
  const RATE_TYPE M,
  ac_channel<Y_TYPE> &out_chan
)
{
  bool busy = false;
#ifndef __SYNTHESIS__
  while (in_chan.available((M.to_uint()!=0)? M.to_uint() : MAX_RATE))
#endif
  {
    busy = true;
    // Downsample the filtered signal by M; that is, keep only every M'th sample.
    // if M==0, then downsample by MAX_RATE
    DOWNSAMPLE_LOOP: for (unsigned i=0; i<MAX_RATE; i++) {
      Y_TYPE   y = in_chan.read();
      if (i==0) { out_chan.write(y); }  // write input stream item to output stream
      if (i==M-1) { break; }            // loop termination if M is not zero (i.e.: not MAX_RATE)
    } // DOWNSAMPLE_LOOP
  } // while
  return busy;
}

