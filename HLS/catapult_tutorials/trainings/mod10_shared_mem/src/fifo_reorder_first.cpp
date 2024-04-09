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
void fifo_reorder_first(
  ac_channel<DATA_TYPE> &i_chan,
  ac_channel<DATA_TYPE> &first_half_chan,
  ac_channel<DATA_TYPE> &second_half_chan)
{

  static unsigned  data_ctr = 0;

#ifndef __SYNTHESIS__
  while (i_chan.available(1))
#endif
  {
    DATA_TYPE  i_data = i_chan.read();

    // sort data by data counter:
    if (data_ctr<HALF_PACKET_LENGTH) {
      first_half_chan.write(i_data);
    } else {
      second_half_chan.write(i_data);
    }
    data_ctr = (data_ctr+1)%PACKET_LENGTH;
  }
}
