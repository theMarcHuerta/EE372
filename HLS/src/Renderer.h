#ifndef _GLOBAL_SIMPLE_H
#define _GLOBAL_SIMPLE_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "ShaderCores.h"
#include "Shader.h"
#include "RayGeneration.h"
#include "PixelAccumulator.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class RenderLooper
{
public:
    RenderLooper() {}

#pragma hls_design interface
void run(ac_channel<img_params> &render_params,
         ac_channel<img_params> &render_params_out,
         ac_channel<LoopIndices> &loopIndicesOut)
    {
        LoopIndices temp_idxs;
        img_params tmp_params;

        tmp_params = render_params.read();
        for (int fy = 0; fy < tmp_params.image_height; fy++){
            temp_idxs.y_pxl = fy;
            for (int fx = 0; fx < tmp_params.image_width; fx++){
                temp_idxs.x_pxl = fx;
                for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
                    temp_idxs.cur_samp = samps;
                    render_params_out.write(temp_idxs)
                    loopIndicesOut.write(temp_idxs); 
                }
            }
        }
    }
};

class RendererWrapper
{
public:
    RendererWrapper(){}
    
#pragma hls_design interface
    void run(ac_channel<sphere_hittable> &spheres_in,
             ac_channel<quad_hittable> &quads_in, 
             ac_channel<img_params> &render_params,
             ac_channel<rgb_t<sfp_9_10>> &output_pxl_sample)
    {
        renderLooper.run(render_params, paramsChanneltoRayGen, loopIndicesChanneltoRayGen);
        rayGeneration.run(loopIndicesChanneltoRayGen, paramsChanneltoRayGen, loopIndicesChanneltoShader, paramsChanneltoShader, rayOut); // TO DO ADD LOOP INDICIES OUT CHANNEL
        shaderCores.run(paramsChanneltoShader, loopIndicesChanneltoShader, rayOut, spheres_in, quads_in, output_pxl_sample);
    }
private:
    RenderLooper renderLooper;
    RayGeneration rayGeneration;
    ShaderCores shaderCores;

    ac_channel<ray<sfp_11_22>> rayOut;
    ac_channel<img_params> paramsChanneltoRayGen;
    ac_channel<img_params> paramsChanneltoShader;
    ac_channel<LoopIndices> loopIndicesChanneltoRayGen;
    ac_channel<LoopIndices> loopIndicesChanneltoShader;
};

#endif


