//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include <iostream>
#include <mc_scverify.h>
#include "packet_reorder.h"

using namespace std;

CCS_MAIN(int argc, char **argv)
{
  cout << __FILE__ << "," << __LINE__ << ":Packet Re-order Shared Memory Example" << endl;

  static ac_channel<DATA_TYPE>  data_in_chan, data_out_chan; // DUT connections
  unsigned error_count = 0;

  const unsigned DATA_COUNT = 3*PACKET_LENGTH;               // let's shoot 3 packets through
  for (unsigned i=0; i<DATA_COUNT; i++) {
    DATA_TYPE  i_data = i;                                  // sequential data for easy checking
    data_in_chan.write(i_data);
  }

  cout << __FILE__ << "," << __LINE__         <<
       ":Calling DUT with " << data_in_chan.size() <<
       " items in the input channel..."       <<
       endl;
  CCS_DESIGN(packet_reorder) (data_in_chan, data_out_chan);
  cout << __FILE__ << "," << __LINE__         <<
       ":DUT has returned with " << data_out_chan.size() <<
       " items in the output channel." <<
       endl;

  if (data_out_chan.size() != DATA_COUNT) {
    error_count++;
    cout << __FILE__ << "," << __LINE__ <<
         ":Error, after DUT call, output channel was expected to have " << DATA_COUNT <<
         " items in the channel, but the number of items is " << data_out_chan.size() <<
         endl;
  }

  // within each packet sent in, the output packets should have the front half and read half reversed:
  for (unsigned i=0; data_out_chan.size()>0; i++) {
    DATA_TYPE  o_data = data_out_chan.read();
    unsigned index_within_packet = i%PACKET_LENGTH;
    DATA_TYPE   exp_data = ((index_within_packet/HALF_PACKET_LENGTH) == 0) ?
                           i+HALF_PACKET_LENGTH : i-HALF_PACKET_LENGTH;
    if (o_data != exp_data) {
      error_count++;
      cout << __FILE__ << "," << __LINE__ <<
           ":Error, data item " << i <<
           " was expected to be " << exp_data <<
           " but dut delivered " << o_data << "." <<
           endl;
    }
  }
  cout << __FILE__ << "," << __LINE__ << ":Packet re-order testbench completed with " <<
       error_count << " errors." << endl;
  CCS_RETURN(0);
}
