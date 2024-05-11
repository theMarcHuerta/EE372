#ifndef MATERIAL_SCATTER_H
#define MATERIAL_SCATTER_H

#include "RTcore.h"
#include "LambertianScatter.h"

class MaterialScatter {
public:
    MaterialScatter(){ start = true; }
    // Function to find the closest hit from channels of spheres and quads
    #pragma hls_design interface
    // #pragma hls_pipeline_init_interval 1
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
    // accumalated_color_out.write(accumalated_color_chan_in.read());
    // if (start){
        // #ifndef __SYNTHESIS__
        // std::cout << "In Scatter"<< std::endl;
        // #endif

        ray tmp_ray = {{0,0,0},{0,0,0},false};
        tmp_ray = ray_in.read();

        // #ifndef __SYNTHESIS__
        // std::cout << "Read Ray from world hit \n"<< std::endl;
        // #endif

        ray scattered_ray = {{0,0,0},{0,0,0},false};

        rgb_in tmp_color_in = {0,0,0};
        tmp_color_in = accumalated_color_chan_in.read();

        rgb_in color_out= {0,0,0};

        rgb_in tmp_atten_in = {0,0,0};
        tmp_atten_in = attenuation_chan_in.read();

        rgb_in attenuation_from_scatter = {0,0,0};

        HitRecord tmp_rec = {{0,0,0}, {0,0,0}, false, 0, {0,0,0}};
        tmp_rec = hit_in.read();

        bool readHit = false;
        readHit = isHit.read();

        rgb_in max_atten = {0, 0, 0};

        rgb_in rgb_color_out = {0, 0, 0};
        rgb_in rgb_atten_out = {0, 0, 0};
        ray ray_scattered_out = {{0,0,0},{0,0,0},false};

        if (!readHit){
            rgb_color_out = tmp_color_in;
            rgb_atten_out = max_atten;
            ray_scattered_out = tmp_ray;
            // attenuation_chan_out.write(max_atten);
            // accumalated_color_out.write(tmp_color_in); // color from background is added in worldhit.h
            // ray_out.write(tmp_ray);
            // std::cout << "NOT hit on pixel " << pixel << std::endl;
            
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
            else { //emission
                rgb_in colorMulOut = {0,0,0};
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
            rgb_color_out = color_out;
            rgb_atten_out = attenuation_from_scatter;
            ray_scattered_out = scattered_ray;
            // std::cout << "was hit on pixel " << pixel << std::endl;
            // attenuation_chan_out.write(attenuation_from_scatter);
            // ray_out.write(scattered_ray);
            // accumalated_color_out.write(color_out);
        }

        attenuation_chan_out.write(rgb_atten_out);
        ray_out.write(ray_scattered_out);
        accumalated_color_out.write(rgb_color_out);

        pixel+=1;

    // }
    }

private:
const rgb_in shader1_atten = {1, 1, 1};
    bool start;
    LambertianScatter lambertianScatter;
    int pixel = 0;
};

#endif