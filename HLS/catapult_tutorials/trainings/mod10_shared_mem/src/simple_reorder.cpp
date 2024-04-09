//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "packet_reorder.h"

// This design illustrates the use of shared memory for re-ordering data
// within a packet of data. The first half packet of data will be swapped
// in order with the second half packet of data. This design fails to achieve

#pragma hls_design top
void packet_reorder(ac_channel<DATA_TYPE> &i_chan,
                    ac_channel<DATA_TYPE> &o_chan)
{

#ifndef __SYNTHESIS__
  // don't do anything without a full packet
  while (i_chan.available(PACKET_LENGTH))
#endif
  {
    DATA_TYPE   mem[HALF_PACKET_LENGTH];

    // save the first half of the packet data:
    LOAD_LOOP:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
      mem[i] = i_chan.read();
    }

    PASSTHRU_LOOP:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
      o_chan.write(i_chan.read());
    }

    UNLOAD_LOOP:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
      o_chan.write(mem[i]);
    }
  }
}
