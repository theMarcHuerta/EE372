//================================================
// File: crc2_gen.cpp
//
// Change History:
//   06/02/2017 - dgb - Initial version
//================================================

// Include data types used by this design
#include "crc2_types.h"

// Include macro definition for CCS_BLOCK
#include <mc_scverify.h>

//================================================
// Function: CRC_Generate
// Description: Generates a "fake" CRC symbol at
//  the end of a packet stream (marked by TLAST)
// Returns: None
//================================================

void CCS_BLOCK(CRC_Generate)(
  ac_channel<Stream_t>         &data_in,
  ac_channel<Stream_t>         &data_out)
{
  static ac_int<11,false> count = 0;
  static Data_t checksum = 0;
#ifndef __SYNTHESIS__
  while (data_in.available(1))
#endif
  {
    Stream_t Data;
    // read the received data
    Data = data_in.read();

    bool isLast = Data.TLAST; // remember last bit
    Data.TLAST = false; // clear from pass-thru word

    // Forward the original data value to next block/process
    data_out.write(Data);

    // Update counter for number of data values processed
    count++;
    // Compute fake CRC
    checksum += Data.TDATA;

    // If last word in packet, write out checksum word now
    if (isLast) {
      Stream_t new_data;
      new_data.TLAST = isLast;
      new_data.TUSER = 0; // unused
      new_data.TDATA = checksum + count;
      data_out.write(new_data);
      checksum = 0;  // clear register
      count = 0;     // reset counter
    }
  }
}

