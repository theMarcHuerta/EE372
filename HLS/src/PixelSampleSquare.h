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
    
    void CCS_BLOCK(run)(img_params &paramsIn, // do i need to make it so i can fetch on command
                        vec3<sfp_3_22> &output_num)
    {
      rand_val.run(rnum1);
      rand_val2.run(rnum2);
      // rand1.run(rnum1);
      // rand2.run(rnum2);
      px = rnum1 - point_five;  // Random offset in the x direction. IS SUPPOSED TO CHOP OFF
      py = rnum2 - point_five;  // Random offset in the y direction.
      vecMul1.run(paramsIn.pixel_delta_u, px, multOut1);
      vecMul2.run(paramsIn.pixel_delta_v, py, multOut2);
      output_num.x = multOut1.x + multOut2.x;
      output_num.y = multOut1.y + multOut2.y;
      output_num.z = multOut1.z + multOut2.z;
    }

  private:

    Rand_val<sfp_3_22> rand_val;
    Rand_val<sfp_3_22> rand_val2;
    Vec3_mult_s<sfp_3_22> vecMul1;
    Vec3_mult_s<sfp_3_22> vecMul2;
    sfp_3_22 rnum1;
    sfp_3_22 rnum2;
    sfp_3_22 px;
    sfp_3_22 py;
    vec3<sfp_3_22> multOut1;
    vec3<sfp_3_22> multOut2;

    const sfp_3_22 point_five = 0.5; // supposed to be .5

};

#endif
