#ifndef RAYGENERATION_H
#define RAYGENERATION_H

#include "RTcore.h"
#include "PixelSampleSquare.h"

class RayGeneration
{
  public:
    RayGeneration(){ 
      state1=375821;
      state2=39251088;
    } // on reset these will be the values
    
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

        buffer_obj_count boc;
        boc.num_quads = tmp_params.num_quads;
        boc.background = tmp_params.background;

        LoopIndices tmp_indices;
        tmp_indices = loopIndicesIn.read();

        ac_fixed<13,12,true> x_pix = tmp_indices.x_pxl;
        ac_fixed<13,12,true> y_pix = tmp_indices.y_pxl;

        vec3<ac_fixed<37, 14, true>> deltaUIndexMultOut;
        vec3<ac_fixed<37, 14, true>> deltaVIndexMultOut;  
        deltUMul.run(tmp_params.pixel_delta_u, x_pix, deltaUIndexMultOut);
        deltVMul.run(tmp_params.pixel_delta_v, y_pix, deltaVIndexMultOut);

        vec3<ac_fixed<38, 15, true>> deltsOut;
        deltAdd.run(deltaUIndexMultOut, deltaVIndexMultOut, deltsOut);
        
        vec3<ac_fixed<39, 16, true>> pxlCenter;
        locDeltsAdd.run(deltsOut, tmp_params.pixel00_loc, pxlCenter);

        vec3<sfp_3_22> pixelSampleSquareOut;
        psq.run(tmp_params, state1, state2, pixelSampleSquareOut);

        // has to extend pxlsq out in bits
        vec3<ac_fixed<40, 17, true>> pixelSample;
        sampleAdd.run(pxlCenter, pixelSampleSquareOut, pixelSample);

        ray tmp_ray;
        tmp_ray.orig = {tmp_params.center.x, tmp_params.center.y, tmp_params.center.z};  // Ray starts at the camera's position.

        rayDiff.run(pixelSample, tmp_ray.orig, tmp_ray.dir);  // Direction from camera to sampled point.

        tmp_ray.camera_ray = true;

        boc.lastsamp = tmp_indices.lastsamp;

        paramsOut.write(boc);
        rayOut.write(tmp_ray);
        // for (int i = 0; i < 9; i++){
        //   if ((i < 8) || ((i == 8) && tmp_indices.lastsamp)) {
        //     paramsOut.write(boc);
        //     rayOut.write(tmp_ray);
        //   }
        // }
      }
    }

  private:
    Vec3_mult_s<ac_fixed<25, 3, true> , ac_fixed<13, 12, true>, ac_fixed<37, 14, true>> deltUMul;
    Vec3_mult_s<ac_fixed<25, 3, true> , ac_fixed<13, 12, true>, ac_fixed<37, 14, true>> deltVMul;

    Vec3_add<ac_fixed<37, 14, true>,  ac_fixed<37, 14, true>,  ac_fixed<38, 15, true>> deltAdd;
    Vec3_add<ac_fixed<38, 15, true>, sfp_11_22, ac_fixed<39, 16, true>> locDeltsAdd;

    Vec3_add<ac_fixed<39, 16, true>, sfp_3_22, ac_fixed<40, 17, true>> sampleAdd;

    Vec3_sub<ac_fixed<40, 17, true>, ac_fixed<21, 11, true>, ac_fixed<34, 11, true>> rayDiff;
    PixelSampleSquare psq;

    ac_int<32, false> state1;
    ac_int<32, false> state2;
    
    // const fp_1_22 point_five = 0.5; // supposed to be .5

};

#endif
