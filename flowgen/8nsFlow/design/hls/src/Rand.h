#ifndef RAND_H
#define RAND_H

#include "RTcore.h"
// #include <math/mgc_ac_trig.h>
#include <ac_math/ac_sincos_cordic.h>
#include <ac_math/ac_arccos_cordic.h>
// #include <cmath>
using namespace ac_math;

class Rand_val {
  public:
    // Generates a random fixed point value between 0 and 1
    void run(ac_int<32, false>& state , ac_fixed<33, 1, false>& out) {
      ac_int<32, false> x = state;
      ac_int<32, false> x1 = x ^ (x << 13);
      ac_int<32, false> x2 = x1 ^ (x1 >> 17);
      ac_int<32, false> x3 = x2^ (x2 << 5);
      state = x3;
      // set lower 32 bits of ac fixed out
      out.set_slc(0, x3);
    }

};

class random_in_unit_sphere {

    public:
    void run(ac_int<32, false>& state1, ac_int<32, false>& state2, vec3<ac_fixed<34, 2, true>>& rand_vec_out){

      ac_fixed<33, 1, false> xi1 = 0;
      ac_fixed<33, 1, false> xi2 = 0;
      // std::cout << "STATE1 " << state1.to_double() << "  STATE2 "<< state2.to_double() << std::endl;
      random1.run(state1, xi1);
      random2.run(state2, xi2);
      // extend the integer part to account for overflow
      ac_fixed<36, 4,  true> fixed_pi = 3.14159; // pi with 32 fractional bits 
      ac_fixed<37, 5,  true> fixed_pi_sq = 9.86960; // pi with 32 fractional bits 
      ac_fixed<36, 4,  true> twoo = 2.0;
      ac_fixed<36, 4,  true> onee = 1.0;
      // SHIFT BY 2
      ac_fixed<36, 4,  true> theta = twoo * xi1; // Uniform distribution from 0 to 2π
      ac_fixed<36, 4,  true> thetatmp = twoo * fixed_pi * xi1; // Uniform distribution from 0 to 2π
      ac_fixed<36, 4,  true> into_acos = (twoo * xi2) - onee;
      // std::cout << "X12 " << xi2.to_double() << std::endl;
      // std::cout << "INTO ACOS " << into_acos.to_double() << std::endl;
      ac_fixed<36, 4,  false> phi;
      ac_fixed<36, 4,  true> phi_signed;
      ac_arccos_cordic(into_acos, phi);// Correctly distribute from -1 to 1 and acos
      phi_signed = phi;

      // std::cout << "ACOS " << into_acos.to_double() << "  CPP ACOS "<< std::acos(into_acos.to_double()) << "  HLS ACOS "<< phi.to_double() << "  END " <<phi_signed << std::endl;
      
      ac_fixed<36, 4,  true> sin_phi;
      ac_fixed<36, 4,  true> cos_phi;
      ac_fixed<36, 4,  true> cos_theta;
      ac_fixed<36, 4,  true> sin_theta;
      ac_sin_cordic(phi_signed, sin_phi);
      ac_cos_cordic(theta, cos_theta);
      ac_sin_cordic(theta, sin_theta);
      ac_cos_cordic(phi_signed, cos_phi);

      // std::cout << "PHI " << phi_signed.to_double() << "  xi1 "<< xi1.to_double() << "  xi2 "<< xi2.to_double() << std::endl;
      // std::cout << "SIN PHI " << sin_phi.to_double() << "  CPP SIN PHI "<< (std::sin(phi_signed.to_double()* 3.14159) ) << std::endl;
      // std::cout << "SIN THETA " << sin_theta.to_double() << "  CPP SIN THETA "<< (std::sin(thetatmp.to_double()) ) << std::endl;
      //   std::cout << "COS THETA " << cos_theta.to_double() << std::endl;
      // std::cout << "COS PHI " << cos_phi.to_double() << std::endl;
      
      ac_fixed<34, 2, true> xs =  sin_phi * cos_theta;
      ac_fixed<34, 2, true> ys =  sin_phi * sin_theta;
      ac_fixed<34, 2, true> zs =  cos_phi;

      //  std::cout << "ACOS " << into_acos.to_double() << std::endl;

      rand_vec_out.x = xs;
      rand_vec_out.y = ys;
      rand_vec_out.z = zs;
    }

    private:
      Rand_val random1;
      Rand_val random2;

};

#endif
