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
void fifo_reorder_second(
  ac_channel<DATA_TYPE>   &first_half_chan,
  ac_channel<DATA_TYPE>   &second_half_chan,
  ac_channel<DATA_TYPE>   &o_data_chan)
{

  static unsigned data_ctr = 0;
#ifndef __SYNTHESIS__
  while (
    ((data_ctr<HALF_PACKET_LENGTH) && (second_half_chan.available(1))) ||
    ((data_ctr>=HALF_PACKET_LENGTH) && (first_half_chan.available(1)))
  )
#endif
  {
    if (data_ctr<HALF_PACKET_LENGTH) {
      o_data_chan.write(second_half_chan.read());
    } else {
      o_data_chan.write(first_half_chan.read());
    }
    data_ctr = (data_ctr+1)%PACKET_LENGTH;
  }
}
