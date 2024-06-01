#ifndef SHADER_H
#define SHADER_H

#include "RTcore.h"
#include "HitRecord.h"
#include "WorldHit.h"
#include "MaterialScatter.h"
#include "LoopDistrib.h"

class Shader
{
public:
    Shader() {
    }
    
    #pragma hls_design interface
    //  #pragma hls_pipeline_init_interval 1
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

        loop_distrib.run(ray_in, params_in, quad_in, attenuation_chan_in, accumalated_color_chan_in,
                        attenuation_chan_to_WorldHit, accumalated_color_to_WorldHit, 
                        rayToLoopOne, rayToLoopTwo, rayToWorldHit, 
                        quad_to_loopone, quad_to_looptwo, quad_max_outone, quad_max_outtwo,
                        params_to_WorldHit);

        intersec_loop_one.hit(quad_to_loopone, rayToLoopOne, quad_max_outone, 
                            quad_hit_anything_outone, rec_quad_outone, closest_so_far_outone);

        intersec_loop_two.hit(quad_to_looptwo, rayToLoopTwo, quad_max_outtwo, 
                            quad_hit_anything_outtwo, rec_quad_outtwo, closest_so_far_outtwo);

        worldHit.hit(rayToWorldHit, params_to_WorldHit, attenuation_chan_to_WorldHit, accumalated_color_to_WorldHit, 
                    quad_hit_anything_outone, quad_hit_anything_outtwo, rec_quad_outone, rec_quad_outtwo, closest_so_far_outone, closest_so_far_outtwo, 
                     attenuation_chan_through, accumalated_color_through, worldRec, rayToScatter, isWorldHit);

        materialScatter.scatter(rayToScatter, worldRec, attenuation_chan_through, accumalated_color_through, isWorldHit,
                                 attenuation_chan_out, accumalated_color_out, ray_out);
    }
private:
    LoopDistrib loop_distrib;
    IntersecLoop intersec_loop_one;
    IntersecLoop intersec_loop_two;
    WorldHit worldHit;
    MaterialScatter materialScatter;

    ac_channel<ray> rayToWorldHit;
    ac_channel<ray> rayToLoopOne;
    ac_channel<ray> rayToLoopTwo;

    ac_channel<rgb_in> attenuation_chan_to_WorldHit;
    ac_channel<rgb_in> accumalated_color_to_WorldHit;

    ac_channel<quad_hittable> quad_to_loopone;
    ac_channel<quad_hittable> quad_to_looptwo;
    ac_channel<uint_11> quad_max_outone;
    ac_channel<uint_11> quad_max_outtwo;

    ac_channel<bool> quad_hit_anything_outone;
    ac_channel<bool> quad_hit_anything_outtwo;

    ac_channel<HitRecord> rec_quad_outone;
    ac_channel<HitRecord> rec_quad_outtwo;

    ac_channel<ac_fixed<47, 17, true>> closest_so_far_outone;
    ac_channel<ac_fixed<47, 17, true>> closest_so_far_outtwo;

    ac_channel<buffer_obj_count> params_to_WorldHit;

    ac_channel<ray> rayToScatter;
    ac_channel<rgb_in> attenuation_chan_through;
    ac_channel<rgb_in> accumalated_color_through;
    ac_channel<bool> isWorldHit;
    ac_channel<HitRecord> worldRec;
};

#endif
