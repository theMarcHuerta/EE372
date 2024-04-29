#ifndef RAND_H
#define RAND_H

#include "RTcore.h"
#include "vec3.h"

class Rand_val {
  public:
    Rand_val(int seed = 375821) : state(seed) {}
    // Generates a random fixed point value between 0 and 1
    void run(ac_int<33, 1, false>& out) {
      ac_int<32, false> x = state;
      x ^= x << 13;
      x ^= x >> 17;
      x ^= x << 5;
      state = x;
      // set lower 32 bits of ac fixed out
      out.set_slc(0, x.slc<32>(0));
    }

  private:
    ac_int<32, false> state;
};

class random_in_unit_sphere {
    Rand_val random1;
    Rand_val random2;
    public:
    voic run(){

      Rand_val.run(xi1);
      Rand_val.run(xi2);

      ax_fixed theta = 2.0 * 3.1415926 * xi1; // Uniform distribution from 0 to 2π
      ax_fixed phi = std::acos(2.0 * xi2 - 1.0); // Correctly distribute from -1 to 1 and acos

      ax_fixed xs = std::sin(phi) * std::cos(theta); // Note the switch of phi and theta here
      ax_fixed ys = std::sin(phi) * std::sin(theta);
      ax_fixed zs = std::cos(phi);

      out = vec3(xs, ys, zs);
    }
}


class Rand_unit_vec {
  random_in_unit_sphere rand_iuv;
  Vec3_unit<ac_fixed<34, 2, true>, ac_fixed<34, 2, true>> unit;
  public:
    // Generates a random unit vector. Useful in simulating diffuse reflections.
    void run(vec3<T>& out) {
      rand_iuv.run();
      unit.run();
    }
};

// class Rand_val {
//   public:
//     Rand_val(int seed = 375821) : state(seed) {}

//     // Generates a random fixed point value between 0 and 1
//     void run(T& out) {
//       ac_int<22, false> feedback = ((state >> 22) ^ (state >> 17) ^ (state >> 3) ^ (state >> 2)) & 1;
//       state = ((state << 1) | feedback);
//       out = 0;
//       out.set_slc(0, state);
//     }

//   private:
//     ac_int<22, false> state;
// };

// template<typename T>
// class Rand_vec {
//   Rand_val<T> rand;
//   public:
//     // Generates a random vec3
//     void run(vec3<T>& out) {
//         rand.run(out.x);
//         rand.run(out.y);
//         rand.run(out.z);
//     }
// };


// class Rand_unit_vec {
//   Rand_vec<ac_fixed<34, 2, true>> rand_vec;
//   Vec3_unit<ac_fixed<34, 2, true>> unit;
//   public:
//     // Generates a random unit vector. Useful in simulating diffuse reflections.
//     void run(vec3<T>& out) {
//         vec3<T> v;
//         rand_vec.run(v);
//         unit.run(v, out);
//     }
// };

#endif
