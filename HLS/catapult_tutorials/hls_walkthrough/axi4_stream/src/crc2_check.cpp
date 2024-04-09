//================================================
// File: crc2_check.cpp
//
// Change History:
//   06/02/2017 - dgb - Initial version
//================================================

// Include data types used by this design
#include "crc2_types.h"

// Include definition of CCS_BLOCKS
#include <mc_scverify.h>

//================================================
// Function: CRC_Check
// Description: Performs a "fake" CRC calculation
//  while receiving a packet and compares the 
//  generated CRC with the received CRC at the
//  end of the packet.
// Returns: None
//================================================

void CCS_BLOCK(CRC_Check)(
  ac_channel<Stream_t>         &data_in,
  ac_channel<Stream_t>         &data_out,
  ac_channel<bool>             &error_out)
{
  static ac_int<11,false> count = 0;
  static Data_t checksum = 0; // fake accumulated checksum
  static bool err_flag = false;
#ifndef __SYNTHESIS__
  while (data_in.available(1))
#endif
  {
    Stream_t Data;
    // read the received data
    Data = data_in.read();

    // Always forward the original data value to next block/process
    data_out.write(Data);
    
    if (Data.TLAST) {
      // Check CRC
      if (checksum+count != Data.TDATA) {
        err_flag = true; // will be high for one word
      }
      checksum = 0; // clear register
      count = 0; // reset counter
    } else {
      err_flag = false;
      // Update counter for number of data values processed
      count++;
      // Compute fake CRC
      checksum += Data.TDATA; 
    }
    error_out.write(err_flag);
  }
}

