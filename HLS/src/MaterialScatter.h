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
#include "LambertianScatter.h"
#include "MetalScatter.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

template<typename T, typename D>
class MaterialScatter {
public:
    MaterialScatter(){}
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design ccore
    // #pragma hls_interface ap_ctrl_none port=return
    void scatter(ac_channel<ray<T>>& ray_in,
                 ac_channel<img_params> &params_in,
                 ac_channel<HitRecord<T>>& hit_in,
                 ac_channel<rgb_in> &attenuation_chan_in,
                 ac_channel<rgb_in> &accumalated_color_chan_in,
                 ac_channel<bool> &isHit,
                 ac_channel<rgb_in> &attenuation_chan_out,
                 ac_channel<rgb_in> &accumalated_color_out,
                 ac_channel<ray<T>> &ray_out
                 )
    {

        ray<T> tmp_ray;
        tmp_ray = ray_in.read();

        ray<T> scattered_ray;

        rgb_in tmp_color_in;
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_in color_out;

        rgb_in tmp_atten_in;
        tmp_atten_in = attenuation_chan_in.read();

        rgb_in attenuation_from_scatter;

        HitRecord<T> tmp_rec;
        tmp_rec = hit_in.read();

        bool readHit;
        readHit = read.write(isHit);

        rgb_in max_atten = {0, 0, 0};

        if (!readHit){
            attenuation_chan_out.write(max_atten);
            accumalated_color_out.write(tmp_color_in); // color from background is added in worldhit.h
            ray_out.write(tmp_ray);
        }
        else {
            if (tmp_rec.mat == LAMBERTIAN){ // lambertian
                lambertianScatter.scatter(tmp_rec, scattered_ray); //sets scattery ray 
                colorMul1.run(tmp_atten_in, rec.color, attenuation_from_scatter); // sets attenuation
                color_out = tmp_color_in;
            }
            else if ((tmp_rec.mat >= METAL_MIN) && (tmp_rec.mat <= METAL_MAX)){ // metal
                D fuzz;
                if (tmp_rec.mat == 1) fuzz = 0;
                if (tmp_rec.mat == 2) fuzz = 0.1875;
                if (tmp_rec.mat == 3) fuzz = 0.375;
                if (tmp_rec.mat == 4) fuzz = 0.625;
                if (tmp_rec.mat == 5) fuzz = 0.8125;
                if (tmp_rec.mat == 6) fuzz = 1;
                bool doesScatter;
                metalScatter.scatter(tmp_ray, tmp_rec, fuzz, scattered_ray, doesScatter);
                if (doesScatter){
                    attenuation_from_scatter = rec.color;
                }
                else {
                    attenuation_from_scatter = max_atten;
                }
                color_out = tmp_color_in;
x            }
            else if (tmp_rec.mat == EMISSIVE){ //emission
                rgb_in colorMulOut;
                colorMul3.run(tmp_atten_in, tmp_rec.color, colorMulOut);
                colorAdd3.run(colorMulOut, tmp_color_in, color_out);
                attenuation_from_scatter = max_atten; // MAKE OUT ATTENTUATION OR FIND A WAY TO STOP REST OF PIPELINE FROM ACCUMULING
                scattered_ray = tmp_ray;
            }
            attenuation_chan_out.write(attenuation_from_scatter);
            ray_out.write(scattered_ray);
            accumalated_color_out.write(color_out);

        }
    }

private:
    HitRecord<T> rec;
    LambertianScatter<T> lambertianScatter;
    MetalScatter<T, D> metalScatter;
    
    Vec3_mult_s<D> colorMul1;

    Vec3_mult_s<D> colorMul3;
    Vec3_add<D> colorAdd3;

};