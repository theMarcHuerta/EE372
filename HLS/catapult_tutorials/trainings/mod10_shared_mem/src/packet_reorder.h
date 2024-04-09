//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef PACKET_REORDER_H_
#define PACKET_REORDER_H_

#include <ac_int.h>
#include <ac_channel.h>

const unsigned DATA_WIDTH         = 12;   // width of each data item in the packet
const unsigned PACKET_LENGTH      = 1024; // number of data items in each packet
const unsigned HALF_PACKET_LENGTH = PACKET_LENGTH/2;
typedef ac_int<DATA_WIDTH,false> DATA_TYPE;

// coding style for shared memory declaration must be a struct containing
// a single arrayed field of ac_int.
typedef struct {
  DATA_TYPE  data[HALF_PACKET_LENGTH];
} HALF_PACKET_TYPE;

// top-level prototype for all approaches:
void packet_reorder(ac_channel<DATA_TYPE> &i_chan, ac_channel<DATA_TYPE> &o_chan);

// subblock prototypes for shared memory approach:
void packet_reorder_first(
  ac_channel<DATA_TYPE>       &i_data_chan,
  ac_channel<HALF_PACKET_TYPE>   &o_packet_chan,
  ac_channel<DATA_TYPE>       &o_data_chan
);
void packet_reorder_second(
  ac_channel<DATA_TYPE>       &i_data_chan,
  ac_channel<HALF_PACKET_TYPE>   &i_packet_chan,
  ac_channel<DATA_TYPE>       &o_data_chan
);

// subblock prototypes for fifo_reorder approach:
void fifo_reorder_first(
  ac_channel<DATA_TYPE>   &i_chan,
  ac_channel<DATA_TYPE>   &first_half_chan,
  ac_channel<DATA_TYPE>   &second_half_chan);
void fifo_reorder_second(
  ac_channel<DATA_TYPE>   &first_half_chan,
  ac_channel<DATA_TYPE>   &second_half_chan,
  ac_channel<DATA_TYPE>   &o_data_chan);

#endif /* PACKET_REORDER_H_ */
