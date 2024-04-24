#ifndef RAYGENERATION_H
#define RAYGENERATION_H

#include "RTcore.h"
#include "PixelSampleSquare.h"

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
        tmp_params = paramsIn.read();

        LoopIndices tmp_indices;
        tmp_indices = loopIndicesIn.read();

        vec3<sfp_11_22> delta_u = {tmp_params.pixel_delta_u.x, tmp_params.pixel_delta_u.y, tmp_params.pixel_delta_u.z};
        vec3<sfp_11_22> delta_v = {tmp_params.pixel_delta_v.x, tmp_params.pixel_delta_v.y, tmp_params.pixel_delta_v.z};

        sfp_11_22 x_pix = tmp_indices.x_pxl;
        sfp_11_22 y_pix = tmp_indices.y_pxl;
        
        deltUMul.run(delta_u, x_pix, deltaUIndexMultOut);
        deltVMul.run(delta_v, y_pix, deltaVIndexMultOut);
        deltAdd.run(deltaUIndexMultOut, deltaVIndexMultOut, deltsOut);
        
        locDeltsAdd.run(deltsOut, tmp_params.pixel00_loc, pixelCenter);
        psq.run(tmp_params, pixelSampleSquareOut);
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
    Vec3_add<sfp_11_22> deltAdd;
    Vec3_add<sfp_11_22> locDeltsAdd;
    Vec3_add<sfp_11_22> sampleAdd;
    Vec3_sub<sfp_11_22> rayDiff;
    PixelSampleSquare psq;

    sfp_3_22 rnum1;
    sfp_3_22 rnum2;
    vec3<sfp_3_22> pixelSampleSquareOut;
    vec3<sfp_11_22> deltaUIndexMultOut;
    vec3<sfp_11_22> deltaVIndexMultOut;
    vec3<sfp_11_22> deltsOut;
    vec3<sfp_11_22> pixelCenter;
    vec3<sfp_11_22> pixelSample;
    const fp_1_22 point_five = 0.5; // supposed to be .5

};

#endif
