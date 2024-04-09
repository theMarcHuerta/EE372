//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

//============================================================================
// Name        : Module7.cpp
// Author      : Jim Corona
// Version     :
// Copyright   :
// Description : decimator function design
//============================================================================

#include <iostream>
#include "decimator.h"
#include <mc_scverify.h>
#include <queue>
using namespace std;

CCS_MAIN(int argc, char **argv)
{
  const unsigned  RATE = 3;            // keep one of every 3 input samples
  assert ((RATE>0)&&(RATE<=MAX_RATE)); // valid range is from 1 to MAX_RATE
  cout << __FILE__ << "," << __LINE__ << ":Decimator Testbench, downsample by " << RATE << "." << endl;

  // define streaming channels for DUT interface:
  static ac_channel<IO_TYPE> in_chan, out_chan;
  queue<IO_TYPE>  expected_sample;     // fifo structure for DUT expected outputs
  // use a binary counter for samples to make output checking easy:
  IO_TYPE   x = 0;                     // input sample to DUT
  IO_TYPE x_quantum;
  x_quantum.set_val<AC_VAL_QUANTUM>(); // sets the LSB, regardless of type

  const unsigned SAMPLE_COUNT = 100*RATE;    // make sure we do enough
  for (unsigned i=0; i<SAMPLE_COUNT; i++, x+=x_quantum) {
    in_chan.write(x);
    // cout << __FILE__ << "," << __LINE__ << ":writing " << x << " to DUT..." << endl;
    if (i%RATE==0) {
      // this will should also come out of the decimator, save for compare
      expected_sample.push(x);
    }
  }

  assert(in_chan.size()==SAMPLE_COUNT);
  assert(out_chan.size()==0);
  cout << __FILE__ << "," << __LINE__ << ":writing " << SAMPLE_COUNT << " samples to DUT..." << endl;
  while (in_chan.size()>0) {
    CCS_DESIGN(decimator)(in_chan, RATE, out_chan);
  }
  cout << __FILE__ << "," << __LINE__ <<
       ":out_chan.size() after return is " << out_chan.size() << "." << endl;

  unsigned error_count = 0;
  if (out_chan.size()!=SAMPLE_COUNT/RATE) {
    error_count++;
    cout << __FILE__ << "," << __LINE__ <<
         ":Error, the number of samples in the output channel of " << out_chan.size() <<
         " is not the expected number " << (SAMPLE_COUNT/RATE) << ".";
    cout << endl;
  }
  while (out_chan.available(1)) {
    IO_TYPE y = out_chan.read();
    // cout << __FILE__ << "," << __LINE__ << ":read " << y << " from DUT." << endl;
    if (y!=expected_sample.front()) {
      error_count++;
      cout << __FILE__ << "," << __LINE__  <<
           ":Error, dut output=" << y <<
           ", expected output=" << expected_sample.front() << endl;
    }
    expected_sample.pop();
  }

  if (error_count==0) {
    cout << __FILE__ << "," << __LINE__ << ":Testbench successfully passed with no mismatches." << endl;
  } else {
    cout << __FILE__ << "," << __LINE__ << ":Testbench failed with " << error_count << " mismatches." << endl;
  }

  CCS_RETURN(0);
}

