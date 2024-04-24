#ifndef SHADER_H
#define SHADER_H

#include "RTcore.h"
#include "HitRecord.h"
#include "WorldHit.h"
#include "MaterialScatter.h"

template<typename T, typename D>
class Shader
{
public:
    Shader(){}
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<buffer_obj_count> &params_in,
                        ac_channel<quad_hittable> &quad_in,
                        ac_channel<sphere_hittable> &sphere_in,
                        ac_channel<ray<T>> &ray_in,
                        ac_channel<rgb_in> &attenuation_chan_in,
                        ac_channel<rgb_in> &accumalated_color_chan_in,
                        ac_channel<rgb_in> &attenuation_chan_out,
                        ac_channel<rgb_in> &accumalated_color_out,
                        ac_channel<rgb_in> &ray_out
                        )
    {   

        worldHit.hit(ray_in, params_in, sphere_in, quad_in, attenuation_chan_in, accumalated_color_chan_in, 
                     attenuation_chan_through, accumalated_color_through, worldRec, rayToScatter, isWorldHit, paramsThrough);
        materialScatter.scatter(rayToScatter, paramsThrough, attenuation_chan_through, accumalated_color_through, isWorldHit,
                                 attenuation_chan_out, accumalated_color_out, ray_out);

    }
private:
    WorldHit<T, D> worldHit;
    MaterialScatter<T, D> materialScatter;

    ac_channel<ray<T>> rayToScatter;
    ac_channel<rgb_in> attenuation_chan_through;
    ac_channel<rgb_in> accumalated_color_through;
    ac_channel<bool> isWorldHit;
    ac_channel<HitRecord<T>> worldRec;
    ac_channel<img_params> paramsThrough;
};

#endif
