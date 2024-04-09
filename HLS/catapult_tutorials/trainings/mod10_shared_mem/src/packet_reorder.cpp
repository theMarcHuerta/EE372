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
// in order with the second half packet of data.
#pragma hls_design top
void packet_reorder(ac_channel<DATA_TYPE> &i_chan,
                    ac_channel<DATA_TYPE> &o_chan)
{

  static ac_channel<DATA_TYPE>     data_interconnect_chan;
  static ac_channel<HALF_PACKET_TYPE>  packet_interconnect_chan;

#ifndef __SYNTHESIS__
  while (i_chan.available(PACKET_LENGTH))
#endif
  {
    packet_reorder_first(i_chan, packet_interconnect_chan, data_interconnect_chan);
    packet_reorder_second(data_interconnect_chan, packet_interconnect_chan, o_chan);
  }
}
