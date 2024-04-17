#ifndef _GLOBAL_SIMPLE_H
#define _GLOBAL_SIMPLE_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "ShaderCores.h"
#include "RayGeneration.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class RenderLooper
{
public:
    RenderLooper() {}

#pragma hls_design interface
void run(ac_channel<img_params> &render_params,
         ac_channel<pxl_params> &pxlparamsOut,
         ac_channel<pxl_deltas> &deltasOut,
         ac_channel<LoopIndices> &loopIndicesOut)
    {
        // -------------------------------
        // Generate the loop indices here for the systolic array.
        // Write the loop indices as well as the params out to channels.
        // Your code starts here
        LoopIndices temp_idxs;
        img_params tmp_params;
        pxl_params tmp_pxl_params;
        pxl_deltas tmp_deltas;
        //outer loop
        tmp_param = render_params.read();
        for (int fy = 0; fy < tmp_param.image_height; fy++){
            temp_idxs.y_pxl = fy;
            for (int fx = 0; fx < tmp_param.image_width; fx++){
                temp_idxs.x_pxl = fx;
                tmp_pxl_params.center = tmp_params.center;
                tmp_pxl_params.pixel00_loc = tmp_params.pixel00_loc;
                tmp_deltas.pixel_delta_u = tmp_params.pixel_delta_u;
                tmp_deltas.pixel_delta_v = tmp_params.pixel_delta_v;
                deltasOut.write(tmp_deltas);
                tmp_pxl_params.write(tmp_tmp_pxl_paramseltas);
                loopIndicesOut.write(temp_idxs); // write idxs adn params
                paramsOut.write(tmp_param);
            }
        }
        // Your code ends here
        // -------------------------------
    }
};

class RendererWrapper
{
public:
    RendererWrapper(){}
    
#pragma hls_design interface
    void run(ac_channel<sphere_hittable[MAX_SPHERES]> &spheres_in,
             ac_channel<quad_hittable[MAX_QUADS]> &quads_in, 
             ac_channel<img_params> &render_params,
            ac_channel<rgb_t> &output_pxl,)
    {
        renderLooper.run(render_params, pxlparams, deltas, paramsChannel, loopIndicesChannel);
        rayGeneration.run(loopIndicesChannel, pxlparams, deltas, bigRay);
        shaderCores.run(bigRay, spheres_in, quads_in, paramsChannel, loopIndicesChannel);
    }
private:
    RenderLooper renderLooper;
    RayGeneration rayGeneration;
    ShaderCores shaderCores;
    ray bigRay;
    ac_channel<img_params> paramsChannel;
    ac_channel<LoopIndices> loopIndicesChannel;
    ac_channel<pxl_params> &pxlparams;
    ac_channel<pxl_deltas> &deltas;
};

#endif


