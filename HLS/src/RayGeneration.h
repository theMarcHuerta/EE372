#ifndef RAYGENERATION_H
#define RAYGENERATION_H

#include "RTcore.h"
#include "PixelSampleSquare.h"

class RayGeneration
{
  public:
    RayGeneration(){ } // on reset these will be the values
    
    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<LoopIndices> &loopIndicesIn,
                        ac_channel<img_params> &paramsIn, // do i need to make it so i can fetch on command
                        ac_channel<buffer_obj_count> &paramsOut,
                        ac_channel<ray> &rayOut)
    {
      #ifndef __SYNTHESIS__
      while(paramsIn.available(1))
      #endif
      {
        img_params tmp_params;
        tmp_params = paramsIn.read();

        boc.num_spheres = tmp_params.num_spheres;
        boc.num_quads = tmp_params.num_quads;
        boc.background = tmp_params.background;
        paramsOut.write(boc);

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

        vec3<sfp_11_22> full_pix_sample = {pixelSampleSquareOut.x, pixelSampleSquareOut.y, pixelSampleSquareOut.z};

        sampleAdd.run(pixelCenter, full_pix_sample, pixelSample);
        ray tmp_ray;
        tmp_ray.orig = {tmp_params.center.x, tmp_params.center.y, tmp_params.center.z};  // Ray starts at the camera's position.
        rayDiff.run(pixelSample, tmp_ray.orig, tmp_ray.dir);  // Direction from camera to sampled point.
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

    buffer_obj_count boc;

};

#endif
