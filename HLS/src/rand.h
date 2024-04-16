#pragma once

#include <ac_int.h>
#include <ac_fixed.h>

#include "types.h"

template<typename T, int tap0, int tap1>
class Rand
{
  public:
    Rand(int seed = 375821) : state(seed) {}
    
    // #pragma hls_design interface
    // void CCS_BLOCK(run)(ac_channel<uint_22> &din, // do i need to make it so i can fetch on command
    //                     ac_channel<sfp_3_22> &output_num)
    // {
    //   uint_22 new_bit = ((state >> 21) ^ (state >> 20)) & 1;  // Taps at positions 22 and 21
    //   state = ((state << 1) | new_bit) & 0x3FFFFF;  // Keep state to 22 bits
    //   output_num.write(state);
    // }

    // #pragma hls_design interface
    T next() {
      rand_type feedback = ((state >> tap0) ^ (state >> tap1)) & 1;  // Taps at positions 22 and 21
      state = ((state << 1) | feedback);
      T return_val = 0;
      return_val.set_slc(0, state);
      return return_val;
    }

  private:
    // uint_22 state;
    rand_type state;
};
