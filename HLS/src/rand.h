#pragma once

#include <ac_int.h>
#include <ac_fixed.h>

#include "types.h"
#include "vec3.h"

template<typename T, int frac_bits, int tap0, int tap1, int tap2, int tap3>
class Rand
{
  public:
    Rand(int seed = 375821) : state(seed) {}

    // Generates a random value between 0 and 1
    T rand() {
      ac_int<frac_bits, false> feedback = ((state >> tap0) ^ (state >> tap1) ^ (state >> tap2) ^ (state >> tap3)) & 1;
      state = ((state << 1) | feedback);
      T return_val = 0;
      return_val.set_slc(0, state);
      return return_val;
    }

    // Generates a random vec3
    vec3<T> rand_vec() {
        T x = rand();
        T y = rand();
        T z = rand();
        return vec3<T>(x, y, z);
    }

    // Generates a random unit vector. Useful in simulating diffuse reflections.
    vec3<T> rand_unit_vec() {
        vec3<T> v = rand_vec();
        return v.unit();
    }

  private:
    ac_int<frac_bits, false> state;
};
