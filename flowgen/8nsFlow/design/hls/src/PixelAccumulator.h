#ifndef PIXEL_ACCUM_H
#define PIXEL_ACCUM_H

#include "RTcore.h"

class PixelAccumulator
{
public:
    PixelAccumulator() {}

#pragma hls_design interface
#pragma hls_pipeline_init_interval 1
void CCS_BLOCK(run)(ac_channel<img_params> &accumulator_parms,
         ac_channel<rgb_in> &pxl_sample,
         ac_channel<rgb_out> &output_pxl_serial)
    {
    #ifndef __SYNTHESIS__
    while(accumulator_parms.available(1))
    #endif
    {
        img_params tmp_params;
        tmp_params = accumulator_parms.read();

        uint_11 stop_point_height = tmp_params.image_end - tmp_params.image_start -1;
        uint_11 stop_point_width= tmp_params.image_width-1;

        // set samples per pixel based on parameters passed in
        ac_int<11, false> spp;
        spp = (tmp_params.samp_per_pxl == 0) ? 32 :
            (tmp_params.samp_per_pxl == 1) ? 64 :
            (tmp_params.samp_per_pxl == 2) ? 256 : 1024;
        
        for (int fy = 0; fy < MAX_IMAGE_HEIGHT; fy++){
            for (int fx = 0; fx < MAX_IMAGE_WIDTH; fx++) {

                // MAKE SURE WE HAVE A readble pxl_sample_in to read
                vec3<ac_fixed<33, 11, false>> accumulation_reg; // SUBJECT TO CHANGE
                ac_fixed<33, 11, false> max_value = 2047.999999761581; // max value we can get with this
                accumulation_reg.x = 0; //r
                accumulation_reg.y = 0; //g
                accumulation_reg.z = 0; //b
                bool red_of = false;
                bool green_of = false;
                bool blue_of = false;

                for (int samps = 0; samps < 1024; samps++){
                    // #ifndef __SYNTHESIS__
                    // while(pxl_sample.available(1))
                    // #endif
                    // {
                    rgb_in cur_sample;
                    cur_sample = pxl_sample.read();
                    accumulation_reg.x = accumulation_reg.x + cur_sample.r;
                    accumulation_reg.y = accumulation_reg.y + cur_sample.g;
                    accumulation_reg.z = accumulation_reg.z + cur_sample.b;
                    // checks for saturations
                    if (tmp_params.samp_per_pxl == 3) {
                        if (accumulation_reg.x[32] == 1) red_of = true;
                        if (accumulation_reg.y[32] == 1) green_of = true;
                        if (accumulation_reg.z[32] == 1) blue_of = true;
                    } else if (tmp_params.samp_per_pxl == 2) {
                        if (accumulation_reg.x[30] == 1) red_of = true;
                        if (accumulation_reg.y[30] == 1) green_of = true;
                        if (accumulation_reg.z[30] == 1) blue_of = true;
                    } else if (tmp_params.samp_per_pxl == 1) {
                        if (accumulation_reg.x[28] == 1) red_of = true;
                        if (accumulation_reg.y[28] == 1) green_of = true;
                        if (accumulation_reg.z[28] == 1) blue_of = true;
                    } else {
                        if (accumulation_reg.x[27] == 1) red_of = true;
                        if (accumulation_reg.y[27] == 1) green_of = true;
                        if (accumulation_reg.z[27] == 1) blue_of = true;
                    }
                    // }
                    if (samps == spp-1) break;
                }
                if (red_of) accumulation_reg.x = max_value;
                if (green_of) accumulation_reg.y = max_value;
                if (blue_of) accumulation_reg.z = max_value;

                vec3<ac_fixed<15, 1, false>> rounded_accuracy_col;
                // STORE TRUNCATED PIXEL COLOR HERE 
                rgb_out pre_output_reg;
                // ac_int<4, false> bit_offset; // USE THIS BIT OFFSET FOR BIT SLICING/BIT SHIFTING INSTEAD OF THE 4 CASES 
                if (tmp_params.samp_per_pxl == 3){
                    rounded_accuracy_col.x.set_slc(0, accumulation_reg.x.slc<15>(18)); // bits [32:18]
                    rounded_accuracy_col.y.set_slc(0, accumulation_reg.y.slc<15>(18));
                    rounded_accuracy_col.z.set_slc(0, accumulation_reg.z.slc<15>(18));
                }
                else if (tmp_params.samp_per_pxl == 2){
                    rounded_accuracy_col.x.set_slc(0, accumulation_reg.x.slc<15>(16)); // bits [30:16]
                    rounded_accuracy_col.y.set_slc(0, accumulation_reg.y.slc<15>(16));
                    rounded_accuracy_col.z.set_slc(0, accumulation_reg.z.slc<15>(16));
                }
                else if (tmp_params.samp_per_pxl == 1){
                    rounded_accuracy_col.x.set_slc(0, accumulation_reg.x.slc<15>(14)); // bits [28:14]
                    rounded_accuracy_col.y.set_slc(0, accumulation_reg.y.slc<15>(14));
                    rounded_accuracy_col.z.set_slc(0, accumulation_reg.z.slc<15>(14));              
                }
                else {
                    rounded_accuracy_col.x.set_slc(0, accumulation_reg.x.slc<15>(13)); // bits [28:13]
                    rounded_accuracy_col.y.set_slc(0, accumulation_reg.y.slc<15>(13));
                    rounded_accuracy_col.z.set_slc(0, accumulation_reg.z.slc<15>(13));             
                }

                vec3<ac_fixed<15, 1, false>> gamme_corrected;
                // Square too to convert linear compoenent to gamma corrected component
                ac_math::ac_sqrt(rounded_accuracy_col.x, gamme_corrected.x);
                ac_math::ac_sqrt(rounded_accuracy_col.y, gamme_corrected.y);
                ac_math::ac_sqrt(rounded_accuracy_col.z, gamme_corrected.z);

                //accounts for overflow (checks the int bit), then takes 8 bits to make it 8 bit rgb
                ac_int<8, false> rgb_max = 255;
                pre_output_reg.r = gamme_corrected.x.slc<1>(14) == 1 ? rgb_max : gamme_corrected.x.slc<8>(6);
                pre_output_reg.g = gamme_corrected.y.slc<1>(14) == 1 ? rgb_max : gamme_corrected.y.slc<8>(6);
                pre_output_reg.b = gamme_corrected.z.slc<1>(14) == 1 ? rgb_max : gamme_corrected.z.slc<8>(6);

                output_pxl_serial.write(pre_output_reg);
                
                if (fx == stop_point_width) break;
            }
            if (fy == stop_point_height) break;
        }
    }
    }
};

#endif
