#ifndef RAND_H
#define RAND_H

#include "RTcore.h"
#include <math/mgc_ac_math.h>

class Rand_val {
  public:
    // Rand_val(int seed = 375821) {}
    // Generates a random fixed point value between 0 and 1
    void run(ac_int<32, false>& state , ac_fixed<33, 1, false>& out) {
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
    void run(ac_int<32, false>& state1, ac_int<32, false>& state2, vec3<ac_fixed<34, 2, true>>& rand_vec_out){
      ac_fixed<33, 1, false> xi1;
      ac_fixed<33, 1, false> xi2;
      random1.run(state1, xi1);
      random2.run(state2, xi2);
      ac_fixed<36, 4,  true> xi1_s = 0;
      ac_fixed<36, 4, true> xi2_s = 0;
      // extend the integer part to account for overflow
      xi1_s.set_slc(0, (xi1).slc<33>(0));
      xi2_s.set_slc(0, (xi2).slc<33>(0));
      ac_fixed<36, 4,  true> fixed_pi = 3.14159; // pi with 32 fractional bits 
      ac_fixed<36, 4,  true> twoo = 2.0;
      ac_fixed<36, 4,  true> onee = 1.0;
      ac_fixed<36, 4,  true> theta = twoo * fixed_pi * xi1; // Uniform distribution from 0 to 2π
      ac_fixed<36, 4,  true> into_acos = twoo * xi2 - onee;
      ac_fixed<36, 4,  true> phi;
      ac_math::arccos(into_acos, phi);// Correctly distribute from -1 to 1 and acos
      
      ac_fixed<36, 4,  true> sin_phi;
      ac_fixed<36, 4,  true> cos_phi;
      ac_fixed<36, 4,  true> cos_theta;
      ac_fixed<36, 4,  true> sin_theta;
      mgc_ac_math::ac_sin(phi, sin_phi);
      mgc_ac_math::ac_cos(theta, cos_theta);
      mgc_ac_math::ac_sin(theta, sin_theta);
      mgc_ac_math::ac_cos(phi, cos_phi);

      ac_fixed<33, 1, false> xs = sin_phi * cos_theta;
      ac_fixed<33, 1, false> ys = sin_phi * sin_theta;
      ac_fixed<33, 1, false> zs = cos_phi;

      rand_vec_out.x = xs;
      rand_vec_out.y = ys;
      rand_vec_out.z = zs;
    }
};


// class Rand_unit_vec {
//   random_in_unit_sphere rand_iuv;
//   Vec3_unit<ac_fixed<34, 2, true>, ac_fixed<34, 2, true>> unit;
//   public:
//     // Generates a random unit vector. Useful in simulating diffuse reflections.
//     void run(vec3<T>& out) {
//       rand_iuv.run();
//       unit.run();
//     }
// };

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
