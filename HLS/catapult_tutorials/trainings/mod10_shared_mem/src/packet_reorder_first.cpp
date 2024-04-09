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
void packet_reorder_first(
  ac_channel<DATA_TYPE> &i_data_chan,
  ac_channel<HALF_PACKET_TYPE> &o_packet_chan,
  ac_channel<DATA_TYPE> &o_data_chan)
{

  // top block guarantees we have a full packet of data
  HALF_PACKET_TYPE mem;
  DATA_TYPE        data;

  // first half packet goes into memory;
  LOAD_MEM:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
    mem.data[i] = i_data_chan.read();
  }
  o_packet_chan.write(mem);

  // second half packet gets passed directly
  PASS_DATA:for (unsigned i=0; i<HALF_PACKET_LENGTH; i++) {
    o_data_chan.write(i_data_chan.read());
  }
}
