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
         ac_channel<rgb_t<sfp_9_10>> &pxl_sample,
         ac_channel<rgb_t<uint_8>> &output_pxl_serial)
    {
        img_params tmp_params;
        tmp_params = accumulator_parms.read();
        for (int fy = 0; fy < tmp_params.image_height; fy++){
            for (int fx = 0; fx < tmp_params.image_width; fx++){
                // MAKE SURE WE HAVE A readble pxl_sample_in to read
                rgb_t<sfp_18_10> accumulation_reg; 
                accumulation_reg.r = 0;
                accumulation_reg.g = 0;
                accumulation_reg.b = 0;
                for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
                    rgb_t<sfp_9_10> cur_sample;
                    cur_sample = pxl_sample.read();
                    accumulation_reg.r = accumulation_reg.r + cur_sample.r;
                    accumulation_reg.g = accumulation_reg.g + cur_sample.g;
                    accumulation_reg.b = accumulation_reg.b + cur_sample.b;
                }
                
                // STORE TRUNCATED PIXEL COLOR HERE 
                rgb_t<uint_8> pre_output_reg;
                // ac_int<4, false> bit_offset; // USE THIS BIT OFFSET FOR BIT SLICING/BIT SHIFTING INSTEAD OF THE 4 CASES 
                if (tmp_params.samp_per_pxl == 3){
                    pre_output_reg.r == accumulation_reg.r[8+10:0+10];
                    pre_output_reg.g == accumulation_reg.g[8+10:0+10];
                    pre_output_reg.b == accumulation_reg.b[8+10:0+10];
                }
                else if (tmp_params.samp_per_pxl == 2){
                    pre_output_reg.r == accumulation_reg.r[8+9:0+9];
                    pre_output_reg.g == accumulation_reg.g[8+9:0+9];
                    pre_output_reg.b == accumulation_reg.b[8+9:0+9];                    
                }
                else if (tmp_params.samp_per_pxl == 1){
                    pre_output_reg.r == accumulation_reg.r[8+8:0+8];
                    pre_output_reg.g == accumulation_reg.g[8+8:0+8];
                    pre_output_reg.b == accumulation_reg.b[8+8:0+8];                    
                }
                else {
                    pre_output_reg.r == accumulation_reg.r[8+6:0+6];
                    pre_output_reg.g == accumulation_reg.g[8+6:0+6];
                    pre_output_reg.b == accumulation_reg.b[8+6:0+6];                    
                }

                output_pxl_serial.write(pre_output_reg);
            }
        }
    }
};