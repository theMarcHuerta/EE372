#ifndef SHADER_H
#define SHADER_H

#include "RTcore.h"
#include "HitRecord.h"
#include "WorldHit.h"
#include "MaterialScatter.h"

class Shader
{
public:
    Shader(){}
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<buffer_obj_count> &params_in,
                        ac_channel<quad_hittable> &quad_in,
                        ac_channel<ray> &ray_in,
                        ac_channel<rgb_in> &attenuation_chan_in,
                        ac_channel<rgb_in> &accumalated_color_chan_in,
                        ac_channel<rgb_in> &attenuation_chan_out,
                        ac_channel<rgb_in> &accumalated_color_out,
                        ac_channel<ray> &ray_out
                        )
    {   
    // #ifndef __SYNTHESIS__
    // while(params_in.available(1))
    // #endif
    // {
        // #ifndef __SYNTHESIS__
        // std::cout << "Running Shader OP "<< std::endl;
        // #endif
        worldHit.hit(ray_in, params_in, quad_in, attenuation_chan_in, accumalated_color_chan_in, 
                     attenuation_chan_through, accumalated_color_through, worldRec, rayToScatter, isWorldHit);
        materialScatter.scatter(rayToScatter, worldRec, attenuation_chan_through, accumalated_color_through, isWorldHit,
                                 attenuation_chan_out, accumalated_color_out, ray_out);
    // }
    }
private:
    WorldHit worldHit;
    MaterialScatter materialScatter;

    ac_channel<ray> rayToScatter;
    ac_channel<rgb_in> attenuation_chan_through;
    ac_channel<rgb_in> accumalated_color_through;
    ac_channel<bool> isWorldHit;
    ac_channel<HitRecord> worldRec;
};

#endif
