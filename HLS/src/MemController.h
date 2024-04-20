#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class MemController
{
public:
    MemController(){}
    
#pragma hls_design interface
    void run(ac_channel<sphere_hittable> &spheres_in,
             ac_channel<quad_hittable> &quads_in, 
             ac_channel<ray> &ray_in,
             ac_channel<LoopIndices> &loop_in,
             ac_channel<img_params> &params_in,
             ac_channel<sphere_hittable> &sphere_out[8],// hope this makes it 8 ac channels
             ac_channel<quad_hittable> &quad_out[8], 
             ac_channel<LoopIndices> &loop_out,
             ac_channel<img_params> &params_out,
             ac_channel<ray> &ray_out
            )
    {
        sphere_hittable tmp_sphere;
        quad_hittable tmp_quad;
        ray tmp_ray;
        LoopIndices temp_idxs;
        img_params tmp_params;
        
        tmp_param = render_params.read();
        for (int fy = 0; fy < tmp_param.image_height; fy++){
            for (int fx = 0; fx < tmp_param.image_width; fx++){
                for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
                    temp_idxs = loop_in.read();
                    tmp_sphere = spheres_in.read();
                    tmp_quad = quads_in.read();
                    tmp_ray = ray_in.read();

                    ray_out.write(tmp_ray);
                    render_params_out.write(temp_idxs)
                    loopIndicesOut.write(temp_idxs); // write idxs adn params
                    #pragma unroll yes
                    for (int i = 0; i < NUM_SHADER_CORES; i++){
                        sphere_out[i].write();
                        quad_out[i].write();
                    }
                }
            }
        }
    }

};

