#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

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
        vec3<ac_int<32, 10, false>> accumulation_reg; // SUBJECT TO CHANGE
        accumulation_reg.x = 0; //r
        accumulation_reg.y = 0; //g
        accumulation_reg.z = 0; //b
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
            }
        }
        
        
        // STORE TRUNCATED PIXEL COLOR HERE 
        rgb_t<uint_8> pre_output_reg;
        // ac_int<4, false> bit_offset; // USE THIS BIT OFFSET FOR BIT SLICING/BIT SHIFTING INSTEAD OF THE 4 CASES 
        if (tmp_params.samp_per_pxl == 3){
            accumulation_reg.x = accumulation_reg.x >> 2;
            accumulation_reg.y = accumulation_reg.y >> 2;
            accumulation_reg.z = accumulation_reg.z >> 2;
        }
        else if (tmp_params.samp_per_pxl == 2){}
        else if (tmp_params.samp_per_pxl == 1){
            accumulation_reg.x = accumulation_reg.x << 2;
            accumulation_reg.y = accumulation_reg.y << 2;
            accumulation_reg.z = accumulation_reg.z << 2;                
        }
        else {
            accumulation_reg.x = accumulation_reg.x << 3;
            accumulation_reg.y = accumulation_reg.y << 3;
            accumulation_reg.z = accumulation_reg.z << 3;                
        }
        pre_output_reg.r.set_slc(21, accumulation_reg.x);
        pre_output_reg.g.set_slc(21, accumulation_reg.y);
        pre_output_reg.b.set_slc(21, accumulation_reg.z);
        output_pxl_serial.write(pre_output_reg);
    }
    }
};