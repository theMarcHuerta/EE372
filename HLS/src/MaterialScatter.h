#ifndef MATERIAL_SCATTER_H
#define MATERIAL_SCATTER_H

#include "RTcore.h"
#include "ray.h"
#include "ShaderCores.h"
#include "RayGeneration.h"
#include "HitRecord.h"
#include "LambertianScatter.h"
#include "MetalScatter.h"

template<typename T, typename D>
class MaterialScatter {
public:
    MaterialScatter(){}
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design interface
    void CCS_BLOCK(scatter)(ac_channel<ray<T>>& ray_in,
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
        readHit = isHit.read();

        rgb_in max_atten = {0, 0, 0};

        if (!readHit){
            attenuation_chan_out.write(max_atten);
            accumalated_color_out.write(tmp_color_in); // color from background is added in worldhit.h
            ray_out.write(tmp_ray);
        }
        else {
            if (tmp_rec.mat == LAMBERTIAN){ // lambertian
                lambertianScatter.run(tmp_rec, scattered_ray); //sets scattery ray

                // set attenuation
                attenuation_from_scatter.r = tmp_atten_in.r * rec.color.r;
                attenuation_from_scatter.g = tmp_atten_in.g * rec.color.g;
                attenuation_from_scatter.b = tmp_atten_in.b * rec.color.b;

                color_out = {tmp_color_in.r, tmp_color_in.g, tmp_color_in.b};
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
                metalScatter.run(tmp_ray, tmp_rec, fuzz, scattered_ray, doesScatter);
                if (doesScatter){
                    attenuation_from_scatter = rec.color;
                }
                else {
                    attenuation_from_scatter = max_atten;
                }
                color_out = tmp_color_in;
            }
            else if (tmp_rec.mat == EMISSIVE){ //emission
                rgb_in colorMulOut;

                colorMulOut.r = tmp_atten_in.r * tmp_rec.color.r;
                colorMulOut.g = tmp_atten_in.g * tmp_rec.color.g;
                colorMulOut.b = tmp_atten_in.b * tmp_rec.color.b;

                color_out.r = colorMulOut.r + tmp_color_in.r;
                color_out.g = colorMulOut.g + tmp_color_in.g;
                color_out.b = colorMulOut.b + tmp_color_in.b;

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
};

#endif
