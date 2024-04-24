#ifndef RAYGENERATION_H
#define RAYGENERATION_H

#include "RTcore.h"
#include "PixelSampleSquare.h"


// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

class RayGeneration
{
  public:
    RayGeneration(){ } // on reset these will be the values
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<LoopIndices> &loopIndicesIn,
                        ac_channel<img_params> &paramsIn, // do i need to make it so i can fetch on command
                        ac_channel<LoopIndices> &loopIndicesOut,
                        ac_channel<img_params> &paramsOut,
                        ac_channel<ray<sfp_11_22>> &rayOut)
    {
      #ifndef __SYNTHESIS__
      while(paramsIn.available(1))
      #endif
      {
        img_params tmp_params;
        tmp_params = render_params.read();
        
        deltUMul.run(loopIndicesIn.x_pxl, tmp_params.pixel_delta_u, deltaUIndexMultOut);
        deltVMul.run(loopIndicesIn.y_pxl, tmp_params.pixel_delta_v, deltaVIndexMultOut);
        deltAdd.run(deltaUIndexMultOut, deltaVIndexMultOut, deltsOut);
        vec3<sfp_11_22> deltsOutBitExt;
        deltsOutBitExt.x = deltsOut.x;
        deltsOutBitExt.y = deltsOut.y;
        deltsOutBitExt.z = deltsOut.z;
        locDeltsAdd.run(deltsOutBitExt, tmp_params.pixel00_loc, pixelCenter);
        pixelSampleSquare.run(tmp_params, pixelSampleSquareOut);
        sampleAdd.run(pixelCenter, pixelSampleSquareOut, pixelSample);
        ray<sfp_11_22> tmp_ray;
        tmp_ray.origin = paramsIn.center;  // Ray starts at the camera's position.
        rayDiff.run(pixelSample, paramsIn.center, tmp_ray.ray_direction);  // Direction from camera to sampled point.
        rayOut.write(tmp_ray);
      }
    }

  private:
    Vec3_mult_s<sfp_11_22> deltUMul;
    Vec3_mult_s<sfp_11_22> deltVMul;
    Vec3_mult_s<sfp_11_22> vecMul2;
    Vec3_add<sfp_4_22> deltAdd;
    Vec3_add<sfp_11_22> locDeltsAdd;
    Vec3_add<sfp_11_22> sampleAdd;
    Vec3_sub<sfp_11_22> rayDiff;

    sfp_3_22 rnum1;
    sfp_3_22 rnum2;
    vec3<sfp_3_22> pixelSampleSquareOut;
    vec3<sfp_11_22> deltaUIndexMultOut;
    vec3<sfp_11_22> deltaVIndexMultOut;
    vec3<sfp_11_22> deltsOut;
    vec3<sfp_11_22> pixelCenter;
    vec3<sfp_11_22> pixelSample;
    static const fp_1_22 point_five = 1<<21; // supposed to be .5

};

#endif
