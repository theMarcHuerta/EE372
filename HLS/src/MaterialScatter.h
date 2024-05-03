#ifndef MATERIAL_SCATTER_H
#define MATERIAL_SCATTER_H

#include "RTcore.h"
#include "LambertianScatter.h"

class MaterialScatter {
public:
    MaterialScatter(){}
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(scatter)(ac_channel<ray>& ray_in,
                 ac_channel<HitRecord>& hit_in,
                 ac_channel<rgb_in> &attenuation_chan_in,
                 ac_channel<rgb_in> &accumalated_color_chan_in,
                 ac_channel<bool> &isHit,
                 ac_channel<rgb_in> &attenuation_chan_out,
                 ac_channel<rgb_in> &accumalated_color_out,
                 ac_channel<ray> &ray_out
                 )
    {

        ray tmp_ray;
        tmp_ray = ray_in.read();

        ray scattered_ray;

        rgb_in tmp_color_in;
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_in color_out;

        rgb_in tmp_atten_in;
        tmp_atten_in = attenuation_chan_in.read();

        rgb_in attenuation_from_scatter;

        HitRecord tmp_rec;
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
                attenuation_from_scatter.r = tmp_atten_in.r * tmp_rec.color.r;
                attenuation_from_scatter.g = tmp_atten_in.g * tmp_rec.color.g;
                attenuation_from_scatter.b = tmp_atten_in.b * tmp_rec.color.b;

                color_out = tmp_color_in;
            }
            else if (tmp_rec.mat == EMISSIVE){ //emission
                rgb_in colorMulOut;
                // should auto truncate the big multiplication
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
    LambertianScatter lambertianScatter;
};

#endif
