//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "decimator.h"

// Include header to support verification
#include <mc_scverify.h>

#pragma hls_design top
void CCS_BLOCK(decimator) (
  ac_channel<IO_TYPE> &in_chan,
  const RATE_TYPE M,
  ac_channel<IO_TYPE> &out_chan
)
{
#ifndef __SYNTHESIS__
  while (in_chan.available((M.to_uint()>0)? M.to_uint() : MAX_RATE))
#endif
  {
    // Downsample the filtered signal by M; that is, keep only every M'th sample.
    // if M==0, then downsample by MAX_RATE
    DOWNSAMPLE_LOOP: for (unsigned i=0; i<MAX_RATE; i++) {
      IO_TYPE  x = in_chan.read();
      if (i==0) { out_chan.write(x); }    // write input stream item to output stream
      if (i==M-1) { break; }           // loop termination if M is not zero (i.e.: MAX_RATE)
    } // DUMP_LOOP
  } // while
}

