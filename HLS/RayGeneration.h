#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"
#include "PizxelSampleSquare.h"


// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

class RayGeneration
{
  public:
    RayGeneration(){ } // on reset these will be the values
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<pxl_deltas> &pixelDeltas, // do i need to make it so i can fetch on command
                        ac_channel<pxl_params> &pixelParams,
                        ac_channel<small_vec3> &pixelSampleSquare,
                        ac_channel<LoopIndices> &loopIndicesIn,
                        ac_channel<ray> &bigRay)
    {
      pxl_deltas tmpDelts;
      tmpDelts.read(pixelDeltas);
      pxl_params tmp_params;
      tmp_params.read(pixelParams);
      pixel_center = tmp_params.pixel00_loc + (loopIndicesIn.x_pxl * tmpDelts.pixel_delta_u) + (loopIndicesIn.y_pxl * tmpDelts.pixel_delta_v);  // Center of the pixel.
      pixel_sample = tmp_params.center + pixelSampleSquare;  // Apply random sampling within the pixel for anti-aliasing.
      ray tmp_ray;
      tmp_ray.origin = center;  // Ray starts at the camera's position.
      tmp_ray.ray_direction = pixel_sample - ray_origin;  // Direction from camera to sampled point.
      bigRay.write(tmp_ray)
    }

  private:

    ws_point_full_fp pixel_center
    ws_point_full_fp pixel_sample
    sfp_3_22 rnum1;
    sfp_3_22 rnum2;
    static const fp_1_22 point_five = 1<<21; // supposed to be .5

};



class RayGenerationWrapper
{
public:
    RayGenerationWrapper(){}
    
#pragma hls_design interface
    void run(ac_channel<pxl_deltas> &pixelDeltas, // do i need to make it so i can fetch on command
              ac_channel<pxl_params> &pixelParams,
              ac_channel<LoopIndices> &loopIndicesIn,
              ac_channel<ray> &bigRay)
    {
      pixelSampleSquare.run(pixelDeltas, outputPxlSampSq, pixelDeltasChannel);
      rayGeneration.run(pixelDeltasChannel, pixelParams, outputPxlSampSq, loopIndicesIn, bigRay)
    }
private:
    RenderLooper renderLooper;
    RayGeneration rayGeneration;
    ShaderCores shaderCores;
    ray bigRay;
    ac_channel<img_params> paramsChannel;
    ac_channel<LoopIndices> loopIndicesChannel;
    ac_channel<small_vec3> pixelSampleSquare
    PixelSampleSquare pixelSampleSquare;
    ac_channel<small_vec3> outputPxlSampSq;
    ac_channel<pxl_deltas> pixelDeltasChannel;
};


