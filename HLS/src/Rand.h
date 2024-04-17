#pragma once

#include "RTcore.h"

template<typename T>
class Rand_val {
  public:
    Rand_val(int seed = 375821) : state(seed) {}

    // Generates a random fixed point value between 0 and 1
    void run(T& out) {
      ac_int<22, false> feedback = ((state >> 22) ^ (state >> 17) ^ (state >> 3) ^ (state >> 2)) & 1;
      state = ((state << 1) | feedback);
      out = 0;
      out.set_slc(0, state);
    }

  private:
    ac_int<22, false> state;
};

template<typename T>
class Rand_vec {
  Rand_val<T> rand;
  public:
    // Generates a random vec3
    void run(vec3<T>& out) {
        rand.run(out.x);
        rand.run(out.y);
        rand.run(out.z);
    }
};

template<typename T>
class Rand_unit_vec {
  Rand_vec<T> rand_vec;
  Vec3_unit<T> unit;
  public:
    // Generates a random unit vector. Useful in simulating diffuse reflections.
    void run(vec3<T>& out) {
        vec3<T> v;
        rand_vec.run(v);
        unit.run(v, out);
    }
};
