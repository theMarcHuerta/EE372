#pragma once

#include <ac_int.h>
#include <ac_fixed.h>

#include "types.h"

template<typename T, int frac_bits, int tap0, int tap1, int tap2, int tap3>
class Rand
{
  public:
    Rand(int seed = 375821) : state(seed) {}

    T next() {
      ac_int<frac_bits, false> feedback = ((state >> tap0) ^ (state >> tap1) ^ (state >> tap2) ^ (state >> tap3)) & 1;
      state = ((state << 1) | feedback);
      T return_val = 0;
      return_val.set_slc(0, state);
      return return_val;
    }

  private:
    ac_int<frac_bits, false> state;
};
