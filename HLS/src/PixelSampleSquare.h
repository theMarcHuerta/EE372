#ifndef _GLOBAL_SIMPLE_H
#define _GLOBAL_SIMPLE_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "rt_core.h"
#include "shader.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>
// TO DO: TEMPLATE IT FOR DIFFERENT SEEDS
/// NEED SOME HANDSHAKE PROTOCOL TO DICTATE WHEN TO FETCH NEXT RAND NUM
class PixelSampleSquare
{
  public:
    PixelSampleSquare(){ } // on reset these will be the values
    
    void CCS_BLOCK(run)(img_params &paramsIn, // do i need to make it so i can fetch on command
                        vec3<sfp_3_22> &output_num)
    {
      rand1.run(rnum1);
      rand2.run(rnum2);
      px = rnum1 - point_five;  // Random offset in the x direction. IS SUPPOSED TO CHOP OFF
      py = rnum2 - point_five;  // Random offset in the y direction.
      vecMul1.run(paramsIn.pixel_delta_u, px, multOut1);
      vecMul2.run(paramsIn.pixel_delta_v, py, multOut2);
      output_num.x = multOut1.x multOut2.x;
      output_num.y = multOut1.y multOut2.y;
      output_num.z = multOut1.z multOut2.z;
    }

  private:

    Rand_val<sfp_3_22>(13301) rand1;
    Rand_val<sfp_3_22>(230682) rand2;
    Vec3_mult_s<sfp_3_22> vecMul1;
    Vec3_mult_s<sfp_3_22> vecMul2;
    sfp_3_22 rnum1;
    sfp_3_22 rnum2;
    sfp_3_22 px;
    sfp_3_22 py;
    vec3<sfp_3_22> multOut1;
    vec3<sfp_3_22> multOut2;

    static const sfp_3_22 point_five = 1<<21; // supposed to be .5

};

#endif
