#ifndef RENDERER_H
#define RENDERER_H

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
#pragma hls_pipeline_init_interval 1
void CCS_BLOCK(run)(ac_channel<img_params> &render_params,
         ac_channel<img_params> &render_params_out,
         ac_channel<LoopIndices> &loopIndicesOut)
        {
        LoopIndices temp_idxs;
        img_params tmp_params;

        tmp_params = render_params.read();
        ac_int<11, false> smp_p_pxl;
        if (tmp_params.samp_per_pxl == 0){
            smp_p_pxl = 32;
        }
        else if(tmp_params.samp_per_pxl == 1){
            smp_p_pxl = 64;
        }
        else if(tmp_params.samp_per_pxl == 2){
            smp_p_pxl = 256;
        }
        else{
            smp_p_pxl = 1024;
        }

        for (int fy = 0; fy < MAX_IMAGE_HEIGHT; fy++){
            temp_idxs.y_pxl = fy;
            // one scanline below
            for (int fx = 0; fx < MAX_IMAGE_WIDTH; fx++){
                temp_idxs.x_pxl = fx;
                for (int samps = 0; samps < MAX_SAMPS_PER_PIXEL; samps++){

                    if (fx == tmp_params.image_width-1 && 
                    fy == tmp_params.image_height-1 && 
                    samps == smp_p_pxl-1){
                        temp_idxs.lastsamp = true;
                    }
                    else {
                        temp_idxs.lastsamp = false;
                    }

                    render_params_out.write(tmp_params);
                    loopIndicesOut.write(temp_idxs); 
                    if (samps == smp_p_pxl-1) break;
                }
                // std::cout << "Pixel " << 240*fy + fx << std::endl;
                if (fx == tmp_params.image_width-1) break;
            }
            if (fy == tmp_params.image_height-1) break;
        }
    }
};

class RendererWrapper
{
public:
    RendererWrapper(){}
    
#pragma hls_design interface
// #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
             ac_channel<img_params> &render_params,
             ac_channel<rgb_in> &output_pxl_sample)
    {
        renderLooper.run(render_params, paramsChanneltoRayGen, loopIndicesChanneltoRayGen);
        rayGeneration.run(loopIndicesChanneltoRayGen, paramsChanneltoRayGen, paramsChanneltoShader, rayOut); // TO DO ADD LOOP INDICIES OUT CHANNEL
        shaderCores.run(quads_in, rayOut, paramsChanneltoShader, output_pxl_sample);
    }
private:
    RenderLooper renderLooper;
    RayGeneration rayGeneration;
    ShaderCores shaderCores;

    ac_channel<ray> rayOut;
    ac_channel<img_params> paramsChanneltoRayGen;
    ac_channel<buffer_obj_count> paramsChanneltoShader;
    ac_channel<LoopIndices> loopIndicesChanneltoRayGen;
};

#endif


