#ifndef WORLD_HIT_H
#define WORLD_HIT_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "ShaderCores.h"
#include "RayGeneration.h"
#include "HitRecord.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class MaterialScatter {
public:
    MaterialScatter(){}
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design ccore
    // #pragma hls_interface ap_ctrl_none port=return
    void scatter(ac_channel<ray<sfp_11_22>>& ray_in,
                 ac_channel<img_params> &params_in,
                 ac_channel<HitRecord<sfp_11_22>>& hit_in,
                 ac_channel<rgb_t<sfp_9_10>> &attenuation_chan_in,
                 ac_channel<rgb_t<sfp_11_10>> &accumalated_color_chan_in,
                 ac_channel<bool> &isHit,
                 ac_channel<rgb_t<sfp_9_10>> &attenuation_chan_out,
                 ac_channel<rgb_t<sfp_11_10>> &accumalated_color_out,
                 ac_channel<ray<sfp_11_22>> &ray_out
                 )
    {

        ray<sfp_11_22> tmp_ray;
        tmp_ray = ray_in.read();

        rgb_t<sfp_9_10> tmp_color_in;
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_t<sfp_11_10> tmp_atten_in;
        tmp_atten_in = attenuation_chan_in.read();

        bool readHit;
        readHit = read.write(isHit);

        if (!isHit){
            attenuation_chan_out.write(tmp_atten_in);
            accumalated_color_out.write(tmp_color_in);
            ray_out.write(tmp_ray);
        }
        else {
            HitRecord<sfp_11_22> tmp_rec;
            tmp_rec = hit_in.read();
            if (tmp_rec.mat == LAMBERTIAN){ // lambertian
                ray scattered_ray;
                bool doesScatter;
                lambertianScatter.scatter();
                if (doesScatter){

                }
                else {
                    sfp_9_10 max_atten(1,1,1); // MAKE CONSTRUCTOR
                    attenuation_chan_out.write(max_atten);
                }
                ray_out.write(tmp_ray);
            }
            else if (tmp_rec.mat == METAL){ // metal
                ray scattered_ray;
                bool doesScatter;
                metalScatter.scatter();
                if (doesScatter){

                }
                else {
                    sfp_9_10 max_atten(1,1,1); // MAKE CONSTRUCTOR
                    attenuation_chan_out.write(max_atten);
                }
                ray_out.write(tmp_ray);
            }
            else if (tmp_rec.mat == EMISSIVE){ //emission
                sfp_11_10 colorMulOut;
                colorMul.run(tmp_atten_in, tmp_rec.color, colorMulOut);
                sfp_11_10 colorAddOut;
                colorAdd.run(colorMulOut, tmp_color_in, colorAddOut);
                accumalated_color_out.write(tmp_color_out);
                attenuation_chan_out.write(tmp_atten_out); // MAKE OUT ATTENTUATION OR FIND A WAY TO STOP REST OF PIPELINE FROM ACCUMULING
                ray_out.write(tmp_ray);
            }
        }
    }

private:
    HitRecord<sfp_11_22> rec;
    bool hitWorld;
    LambScatter lambertianScatter;
    MetalScatter metalScatter;

    Vec3_mult_s<sfp_11_10> colorMul3;
    Vec3_add<sfp_11_10> colorAdd3;

    rgb_t<sfp_11_10> backgroundColor;
    HitRecord rec;
};