#ifndef PIXEL_SAMPLE_H
#define PIXEL_SAMPLE_H

#include "Rand.h"
#include "RTcore.h"
#include "Shader.h"


// TO DO: TEMPLATE IT FOR DIFFERENT SEEDS
/// NEED SOME HANDSHAKE PROTOCOL TO DICTATE WHEN TO FETCH NEXT RAND NUM
class PixelSampleSquare
{
  public:
    PixelSampleSquare(){ } // on reset these will be the values
    
    void CCS_BLOCK(run)(img_params &paramsIn,
                        ac_int<32, false>& state1,
                        ac_int<32, false>& state2,
                         // do i need to make it so i can fetch on command
                        vec3<sfp_3_22> &output_num)
    {
      ac_int<33, 1, false> rnum1;
      ac_int<33, 1, false> rnum2; 
      rand_val.run(state1, rnum1);
      rand_val2.run(state2, rnum2);
      ac_int<34, 2, true> rnum1_s = 0;
      ac_int<34, 2, true> rnum2_s = 0; 
      // sets the lower 32 bits of the signed number to the rand
      rnum1_s.set_slc(0, (rnum1).slc<33>(0));
      rnum2_s.set_slc(0, (rnum2).slc<33>(0));

      ac_int<34, 2, true> px = rnum1_s - point_five;  // Random offset in the x direction. IS SUPPOSED TO CHOP OFF
      ac_int<34, 2, true> py = rnum2_s - point_five;  // Random offset in the y direction.

      vec3<sfp_3_22> multOut1;
      vec3<sfp_3_22> multOut2;

      // HOPE THE OUTPUT TRUNCATES THE OUTPUT TO 22 FRACTIONAL BITS CORRECTLY
      vecMul1.run(paramsIn.pixel_delta_u, px, multOut1);
      vecMul2.run(paramsIn.pixel_delta_v, py, multOut2);

      output_num.x = multOut1.x + multOut2.x;
      output_num.y = multOut1.y + multOut2.y;
      output_num.z = multOut1.z + multOut2.z;
    }

  private:

    Rand_val rand_val;
    Rand_val rand_val2;

    Vec3_mult_s<sfp_3_22, ac_int<34, 2, true>, sfp_3_22> vecMul1;
    Vec3_mult_s<sfp_3_22, ac_int<34, 2, true>, sfp_3_22> vecMul2;

    const sfp_3_22 point_five = 0.5; // supposed to be .5

};

#endif
