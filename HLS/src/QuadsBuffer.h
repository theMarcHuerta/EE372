#ifndef QUADSBUFFER_H
#define QUADSBUFFER_H

#include "RTcore.h"

template <int size>
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
        buffer_params params = paramsIn.read();
        
        // Temporary storage for quads
        chanStruct<quad_hittable, size> buffer;
        
        // Writing to buffer
        for (int i = 0; i < params.num_quads; i++) {
            buffer.data[i] = quads_in.read();
        }

        // Reading from buffer multiple times based on image parameters
        for (int fy = 0; fy < params.image_height; fy++) {
            for (int fx = 0; fx < params.image_width; fx++) {
                for (int samps = 0; samps < params.samp_per_pxl; samps++) {
                    for (int bounces = 0; bounces < 8; i++){
                        for (int i = 0; i < params.num_quads; i++) {
                            quads_out.write(buffer.data[i]);
                        }
                    }
                }
            }
        }
    }
};

#endif

