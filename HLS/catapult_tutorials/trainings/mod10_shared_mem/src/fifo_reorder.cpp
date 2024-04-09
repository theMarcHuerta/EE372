//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "packet_reorder.h"

// This design illustrates the use of an interconnect FIFO for re-ordering data
// within a packet of data. Two interconnect channels are used, one carries
// the data from the first half of the packet and the second channel carries
// the data from the second half of the packet of data.
//
// packet_reorder_first sorts the data into the two channels
// packet_reorder_second combines the two streams, first the second half packet data,
// then the first
// The first_half_chan1 needs to have a FIFO large enough to hold a half packet of data

#pragma hls_design top
void packet_reorder(ac_channel<DATA_TYPE> &i_chan, ac_channel<DATA_TYPE> &o_chan)
{

  static ac_channel<DATA_TYPE>  first_half_chan;
  static ac_channel<DATA_TYPE>  second_half_chan;

  fifo_reorder_first(i_chan, first_half_chan, second_half_chan);
  fifo_reorder_second(first_half_chan, second_half_chan, o_chan);
}
