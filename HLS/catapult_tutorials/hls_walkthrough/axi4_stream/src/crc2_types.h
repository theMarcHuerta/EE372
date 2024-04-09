//================================================
// File: crc2_types.cpp
//
// Change History:
//   06/02/2017 - dgb - Initial version
//================================================

#ifndef _INCLUDED_CRC_TYPES_H_
#define _INCLUDED_CRC_TYPES_H_

#include <ac_int.h>
#include <ac_channel.h>

#define AXI_BUS_WIDTH 8
typedef ac_int<AXI_BUS_WIDTH,false> Data_t;
typedef ac_int<1,false> User_t; // 1-bit sideband data (unused)

// Type definition for the datastream I/O
struct Stream_t {
  Data_t   TDATA;
  User_t   TUSER;
  bool     TLAST;
};

// Function declarations

// Insert CRC2 at end of packet
void CRC_Generate(
  ac_channel<Stream_t>         &data_in,
  ac_channel<Stream_t>         &data_out);

// Check CRC2 at end of packet
void CRC_Check(
  ac_channel<Stream_t>         &data_in,
  ac_channel<Stream_t>         &data_out,
  ac_channel<bool>             &error_out);

// Top-level block that instantiates CRC_Generate and CRC_Check
void CRC_Top(
  ac_channel<Stream_t>         &data_in, // input stream without CRC
  ac_channel<Stream_t>         &data_out, // output stream, checked, with CRC
  ac_channel<bool>             &error_out);

#endif
