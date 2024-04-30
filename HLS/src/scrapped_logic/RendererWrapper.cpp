#ifndef RENDERER_TOP
#define RENDERER_TOP

#include "RTcore.h"
#include "ShaderCores.h"
#include "Shader.h"
#include "RayGeneration.h"
#include "PixelAccumulator.h"


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
                    render_params_out.write(tmp_params);
                    loopIndicesOut.write(temp_idxs); 
                }
            }
        }
    }
};

#pragma hls_design top
class RendererWrapper
{
public:
    RendererWrapper(){}
    
    #pragma hls_design interface
    void run(ac_channel<sphere_hittable> &spheres_in,
             ac_channel<quad_hittable> &quads_in, 
             ac_channel<img_params> &render_params,
             ac_channel<rgb_in> &output_pxl_sample)
    {
        renderLooper.run(render_params, paramsChanneltoRayGen, loopIndicesChanneltoRayGen);
        rayGeneration.run(loopIndicesChanneltoRayGen, paramsChanneltoRayGen, paramsChanneltoShader, rayOut); // TO DO ADD LOOP INDICIES OUT CHANNEL
        shaderCores.run(spheres_in, quads_in, rayOut, paramsChanneltoShader, output_pxl_sample);
    }
private:
    RenderLooper renderLooper;
    RayGeneration rayGeneration;
    ShaderCores shaderCores;

    ac_channel<ray<sfp_11_22>> rayOut;
    ac_channel<img_params> paramsChanneltoRayGen;
    ac_channel<buffer_obj_count> paramsChanneltoShader;
    ac_channel<LoopIndices> loopIndicesChanneltoRayGen;
};

#endif


