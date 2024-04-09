//================================================
// File: crc2_top.cpp
//
// Change History:
//   06/02/2017 - dgb - Initial version
//================================================

// Include data types used by this design
#include "crc2_types.h"

// Include macro definition for CCS_BLOCK
#include <mc_scverify.h>

#pragma design
void CCS_BLOCK(CRC_Top)(
  ac_channel<Stream_t>         &data_in, // input stream without CRC
  ac_channel<Stream_t>         &data_out, // output stream, checked, with CRC
  ac_channel<bool>             &error_out)
{
  static ac_channel<Stream_t>  buffer;
  CRC_Generate(data_in,buffer);
  CRC_Check(buffer,data_out,error_out);
}

