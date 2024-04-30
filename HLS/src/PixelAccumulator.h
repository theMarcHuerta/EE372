#ifndef PIXEL_ACCUM_H
#define PIXEL_ACCUM_H

#include "RTcore.h"


class PixelAccumulator
{
public:
    PixelAccumulator() {}

#pragma hls_design interface
void run(ac_channel<img_params> &accumulator_parms,
         ac_channel<rgb_in> &pxl_sample,
         ac_channel<rgb_out> &output_pxl_serial)
    {
    #ifndef __SYNTHESIS__
    while(accumulator_parms.available(1))
    #endif
    {
        img_params tmp_params;
        tmp_params = accumulator_parms.read();
        // MAKE SURE WE HAVE A readble pxl_sample_in to read
        vec3<ac_fixed<33, 11, false>> accumulation_reg; // SUBJECT TO CHANGE
        ac_fixed<33, 11, false> max_value = 2047.999999761581; // max value we can get with this
        accumulation_reg.x = 0; //r
        accumulation_reg.y = 0; //g
        accumulation_reg.z = 0; //b
        bool red_of = false;
        bool green_of = false;
        bool blue_of = false;
        for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
            #ifndef __SYNTHESIS__
            while(pxl_sample.available(1))
            #endif
            {
            rgb_in cur_sample;
            cur_sample = pxl_sample.read();
            accumulation_reg.x = accumulation_reg.x + cur_sample.r;
            accumulation_reg.y = accumulation_reg.y + cur_sample.g;
            accumulation_reg.z = accumulation_reg.z + cur_sample.b;
            // checks for saturations
            if (accumulation_reg.x[32] == 1) red_of = true;
            if (accumulation_reg.y[32] == 1) green_of = true;
            if (accumulation_reg.z[32] == 1) blue_of = true;
            }
        }
        if (red_of) accumulation_reg.x = max_value;
        if (green_of) accumulation_reg.y = max_value;
        if (blue_of) accumulation_reg.z = max_value;

        vec3<ac_fixed<15, 1, false>> rounded_accuracy_col;
        // STORE TRUNCATED PIXEL COLOR HERE 
        rgb_out pre_output_reg;
        // ac_int<4, false> bit_offset; // USE THIS BIT OFFSET FOR BIT SLICING/BIT SHIFTING INSTEAD OF THE 4 CASES 
        if (tmp_params.samp_per_pxl == 3){
            rounded_accuracy_col.x = accumulation_reg.x.slc<15>(18); // bits [32:18]
            rounded_accuracy_col.y = accumulation_reg.y.slc<15>(18);
            rounded_accuracy_col.z = accumulation_reg.z.slc<15>(18);
        }
        else if (tmp_params.samp_per_pxl == 2){
            rounded_accuracy_col.x = accumulation_reg.x.slc<15>(16); // bits [30:16]
            rounded_accuracy_col.y = accumulation_reg.y.slc<15>(16);
            rounded_accuracy_col.z = accumulation_reg.z.slc<15>(16);
        }
        else if (tmp_params.samp_per_pxl == 1){
            rounded_accuracy_col.x = accumulation_reg.x.slc<15>(14); // bits [28:14]
            rounded_accuracy_col.y = accumulation_reg.y.slc<15>(14);
            rounded_accuracy_col.z = accumulation_reg.z.slc<15>(14);              
        }
        else {
            rounded_accuracy_col.x = accumulation_reg.x.slc<15>(13); // bits [28:13]
            rounded_accuracy_col.y = accumulation_reg.y.slc<15>(13);
            rounded_accuracy_col.z = accumulation_reg.z.slc<15>(13);             
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
    }
    }
};

// #ifndef __ACCUM__
// 2 #define __ACCUM__
// 3 #include <ac_int.h>
// 4 #include “../../helper_classes/src/log2ceil.h”
// 5 template<int W, bool S, int N>
// 6 ac_int<W+LOG2_CEIL<N>::val,S> accumulate(ac_int<W,S> din[N]){
// 7 ac_int<W+LOG2_CEIL<N>::val,S> acc = 0;
// 8
// 9 ACCUM:for(int i=0;i<N;i++){
// 10 acc += din[i];
// 11 }
// 12 return acc;
// 13 }

#endif
