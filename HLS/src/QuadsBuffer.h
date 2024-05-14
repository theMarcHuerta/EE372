#ifndef QUADSBUFFER_H
#define QUADSBUFFER_H

#include "RTcore.h"

template <int size = MAX_QUADS_IN_BUFFER>
class QuadBuffer {
public:
    QuadBuffer(){}

    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<buffer_params> &paramsIn,
                        ac_channel<quad_hittable> &quads_out)
    {
        // Reading buffer parameters
        #ifndef __SYNTHESIS__
        while (paramsIn.available(1))
        #endif
        {
            buffer_params params = paramsIn.read();
            
            // Temporary storage for quads
            chanStruct<quad_hittable, size> buffer;
            
            // Writing to buffer
            // #pragma hls_pipeline_init_interval 1
            for (int i = 0; i < MAX_QUADS_IN_BUFFER-1; i++) {
                buffer.data[i] = quads_in.read();
                if (i == params.num_quads-1) break; // check if its odd or even
            }


            ac_int<11, false> spp;
            spp = (params.samp_per_pxl == 0) ? 32 :
                (params.samp_per_pxl == 1) ? 64 :
                (params.samp_per_pxl == 2) ? 256 : 1024;



            #define MAXLOOP 34359738369
            ac_int<22, false> pixels_in_img = (params.image_height) * (params.image_width);
            ac_int<33, false> tot_samples_in_img = pixels_in_img * (spp);
            ac_int<36, false> tot_read_requests = (tot_samples_in_img << 3) + 1; // multiply by 8 plus 1 for the extra end case bounce
            // Reading from buffer multiple times based on image parameters
            #pragma hls_pipeline_init_interval 1
            for (ac_int<43,false> read_request = 0; read_request < MAXLOOP; read_request += 1){
                for (ac_int<7, false> quad_idx = 0; quad_idx < MAX_QUADS_IN_BUFFER; quad_idx += 1) {
                    quads_out.write(buffer.data[quad_idx]);
                    if (quad_idx == params.num_quads-1) break;
                }
                if (read_request == tot_read_requests-1) break;
            }
        }
    }
};

#endif

