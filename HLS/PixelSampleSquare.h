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
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<pxl_deltas> &pixelDeltas, // do i need to make it so i can fetch on command
                        ac_channel<small_vec3> &output_vec,
                        ac_channel<pxl_deltas> &output_delts)
    {
      rand1.run(, rnum1);
      rand2.run(, rnum2);
      pxl_deltas tmp_delts;
      tmp_delts = pixelDeltas.read();
      sfp_3_22 px = point_five + rnum1;  // Random offset in the x direction. IS SUPPOSED TO CHOP OFF
      sfp_3_22 py = point_five + rnum2;  // Random offset in the y direction.
      small_vec3 tmp_out = (px * tmp_delts.pixel_delta_u) + (py * tmp_delts.pixel_delta_v);  // Adjust by pixel size. NEED OPERATOR OVERLOADER HERE OR JUST HAND DO IT
      // also for the above statement i hope it can convert to full on 11-22 format
      output_num.write(tmp_out)
      output_delts.write(tmp_delts);
    }

  private:

    Rand(13301) rand1;
    Rand(230682) rand2;
    sfp_3_22 rnum1;
    sfp_3_22 rnum2;
    static const fp_1_22 point_five = 1<<21; // supposed to be .5

};

#endif
