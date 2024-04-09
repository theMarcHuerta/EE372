//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "packet_reorder.h"

#pragma hls_design block
void packet_reorder_second(
  ac_channel<DATA_TYPE> &i_data_chan,
  ac_channel<HALF_PACKET_TYPE> &i_packet_chan,
  ac_channel<DATA_TYPE> &o_data_chan)
{

  // top-level guarantees we have full packet at input

  // first write out second half packet of data:
  PASS_DATA:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
    o_data_chan.write(i_data_chan.read());
  }

  // same shared memory as in first subblock
  HALF_PACKET_TYPE mem;

  // then write out first half-packet of data held in shared memory:
  i_packet_chan.read(mem);
  UNLOAD_MEM:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
    o_data_chan.write(mem.data[i]);
  }
}
