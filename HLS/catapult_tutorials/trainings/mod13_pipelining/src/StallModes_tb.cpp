//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include <iostream>
using namespace std;
#include "StallModes.h"
#include <list>
#include <mc_scverify.h>

CCS_MAIN(int argc, char **argv)
{
  cout << __FILE__ << "," << __LINE__ << ":Stall Modes Stimulus Generator" << endl;

#ifdef CCS_SCVERIFY
  bool random_stall = false;
  if (argc>1) {
    string str(argv[1]);
    if (str == "-r") {
      cout << __FILE__ << "," << __LINE__ << ":Testbench will run with randomized stalls." << endl;
      random_stall = true;
    }
  }
#endif

  const unsigned GROUP_COUNT = 16;

  ac_channel<INPUT_TYPE>  x_chan;
  ac_channel<OUTPUT_TYPE>  f_chan;

  // stimulus consists of one-hot and two boundary conditions:
  const unsigned   STIMULUS_COUNT = INPUT_TYPE::width+2;
  INPUT_TYPE      stimulus[STIMULUS_COUNT];

  // build up our stimulus value set:
  for (unsigned i=0, bit=0; i<INPUT_TYPE::width; i++, bit=(bit+1)%INPUT_TYPE::width) {
    // one-hot encoding:
    ac_int<INPUT_TYPE::width,false>  x_ac_int = 0;
    x_ac_int[bit] = 1;
    INPUT_TYPE  x;
    x.set_slc(0,x_ac_int);
    stimulus[i] = x;
  }

  // throw in a couple of boundary conditions:
  {
    INPUT_TYPE  x;
    x.set_val<AC_VAL_MAX>();
    stimulus[INPUT_TYPE::width] = x;  // all bits on
    x.set_val<AC_VAL_MIN>();
    stimulus[INPUT_TYPE::width+1] = x;  // zero divide
  }

  // repetitively send GROUP_COUNT sets of our stimulus value set:
  for (unsigned grp_ix=0; grp_ix<GROUP_COUNT; grp_ix++) {
    for (unsigned i=0; i<STIMULUS_COUNT; i++) {
      x_chan.write(stimulus[i]);
    }
  } // grp_ix

#ifdef CCS_SCVERIFY
  if (random_stall) {
    x_chan_wait_ctrl.mode      = mc_wait_ctrl::UNIFORM;
    x_chan_wait_ctrl.cycles    = 4;
    x_chan_wait_ctrl.elements  = mc_wait_ctrl::RANDOM_INTERVAL;
    x_chan_wait_ctrl.interval  = 8;
    f_chan_wait_ctrl.mode      = mc_wait_ctrl::UNIFORM;
    f_chan_wait_ctrl.cycles    = 4;
    f_chan_wait_ctrl.elements  = mc_wait_ctrl::RANDOM_INTERVAL;
    f_chan_wait_ctrl.interval  = 8;
  }
#endif

  StallModes(x_chan, f_chan);  // call dut

  assert(f_chan.size()==GROUP_COUNT*STIMULUS_COUNT);

  // let's read and look at the values:
  for (unsigned grp_ix=0; grp_ix<GROUP_COUNT; grp_ix++) {
    for (unsigned i=0; i<STIMULUS_COUNT; i++) {
      cout << __FILE__ << "," << __LINE__ << ":x=" << stimulus[i] << ", f=" << f_chan.read() << endl;
    }
  }

  cout << __FILE__ << "," << __LINE__ << ":End of Stall Modes Stimulus Generator." << endl;

  CCS_RETURN(0);
}
