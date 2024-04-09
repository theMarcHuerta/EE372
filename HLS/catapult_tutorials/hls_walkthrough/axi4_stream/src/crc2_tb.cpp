//================================================
// File: crc2_tb.cpp
//
// Change History:
//   06/02/2017 - dgb - Initial version
//================================================

// Include data types used by this design
#include "crc2_types.h"

// Include macro definition for CCS_BLOCK
#include <mc_scverify.h>

#include <iostream>
using namespace std;

#define MAX_TB_ITERATIONS 2

// -----------------------------------------
// Function: fillChannel
// Description: Fill the channel 'chout' with 'packetSize' words of random data
// -----------------------------------------
void fillChannel(ac_channel<Stream_t> &chout, unsigned short packetSize)
{
  // Load words into packet
  unsigned short packetsIn = packetSize;
  Stream_t Data;
  while (packetsIn-- > 0) {
    Data.TDATA = 0; Data.TUSER = 0; Data.TLAST = false;
    Data.TDATA = (int)(rand()&255);
    if (packetsIn==0) Data.TLAST = true;
    chout.write(Data);
  }
}

// -----------------------------------------
// Function: dumpChannel
// Description: Dump the contents of the channel 'ch'. Optional message string
//    is prepended to the output.
// -----------------------------------------
void dumpChannel(ac_channel<Stream_t> &ch, const char *msg) {
  // Dump input packet
  for (unsigned int x=0; x<ch.debug_size(); x++) {
    if (msg) cout << msg << " packet[" << x << "] Data=" << ch[x].TDATA << " User=" << ch[x].TUSER << " last=" << ch[x].TLAST << endl;
    else     cout <<        " packet[" << x << "] Data=" << ch[x].TDATA << " User=" << ch[x].TUSER << " last=" << ch[x].TLAST << endl;
  }
}

// -----------------------------------------
// Function: dumpChannel
// Description: Dump the contents of the channel 'ch'. Optional message string
//    is prepended to the output.
// -----------------------------------------

CCS_MAIN(int argc, char *argv[])
{
  if (argc < 2) {
    cerr << "Usage: a.out <packetSize>" << endl;
    cerr << "   where: packetSize specifies the number of packets to send before inserting CRC2 checksum." << endl;
    cerr << "              Max value for packetSize is 16." << endl;
    CCS_RETURN(-1);
  }
  unsigned short packetSize = (unsigned short)atoi(argv[1]);
  if (packetSize > 16) packetSize = 16;

  ac_channel<bool>     error_out;
  ac_channel<Stream_t> stream_in;
  ac_channel<Stream_t> stream_crc_out;
  Stream_t Data;

  // Main testbench iteration loop
  for (unsigned int iter = 1; iter <= MAX_TB_ITERATIONS; iter++) {
    cout << "-- TESTBENCH ITERATION " << iter << endl;

    // Load words into packet
    fillChannel(stream_in,packetSize);
    dumpChannel(stream_in,"Pre-Generate");

    // Generate CRC2
    CRC_Top(stream_in,stream_crc_out,error_out);
    dumpChannel(stream_crc_out,"Post-Check");

    // Sanity check - should be one extra word
    if (stream_crc_out.debug_size() != packetSize+1) {
      cerr << "Error: CRC2_Generate did not append checksum word" << endl;
    }

    while (stream_crc_out.available(1)) { (void) stream_crc_out.read(); }
    while (error_out.available(1)) { (void) error_out.read(); }
  }
  CCS_RETURN(0);
}


