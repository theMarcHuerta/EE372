#ifndef QUADSBUFFER_H
#define QUADSBUFFER_H

#include "RTcore.h"

template <int size>
class QuadBuffer {
public:
    QuadBuffer(){}

    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<ac_int<12,false>> &quads_in, 
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
            #pragma hls_pipeline_init_interval 1
            for (int i = 0; i < MAX_QUADS_IN_BUFFER-1; i++) {
                quad_hittable quad_to_sram;
                // CORNER POINT
                quad_to_sram.corner_pt.x.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.corner_pt.y.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.corner_pt.z.set_slc(0, (inputChannel.read())<12>(0));
                // U VECTOR
                quad_to_sram.u.x.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.u.y.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.u.z.set_slc(0, (inputChannel.read())<12>(0));
                // V VECTOR
                quad_to_sram.v.x.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.v.y.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.v.z.set_slc(0, (inputChannel.read())<12>(0));
                // MATERIAL TYPE
                quad_to_sram.mat_type.set_slc(0, (inputChannel.read())<3>(0));
                // IS INVISIBLE
                ac_int<1,false> isinvis.set_slc(0, (inputChannel.read())<1>(0));
                quad_to_sram.is_invis = isinvis ? true : false;
                // NORMAL VECTOR
                // x
                quad_to_sram.normal.x.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.normal.x.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.normal.x.set_slc(24, (inputChannel.read())<2>(0));
                // y
                quad_to_sram.normal.y.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.normal.y.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.normal.y.set_slc(24, (inputChannel.read())<2>(0));
                // z
                quad_to_sram.normal.z.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.normal.z.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.normal.z.set_slc(24, (inputChannel.read())<2>(0));
                // W VECTOR
                //x
                quad_to_sram.w.x.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.w.x.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.w.x.set_slc(24, (inputChannel.read())<1>(0));
                // y
                quad_to_sram.w.y.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.w.y.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.w.y.set_slc(24, (inputChannel.read())<1>(0));
                // z
                quad_to_sram.w.z.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.w.z.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.w.z.set_slc(24, (inputChannel.read())<1>(0));
                // D PLANE
                quad_to_sram.w.d_plane.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.w.d_plane.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.w.d_plane.set_slc(24, (inputChannel.read())<7>(0));
                // QUAD COLOR
                //r
                quad_to_sram.quad_color.r.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.quad_color.r.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.quad_color.r.set_slc(24, (inputChannel.read())<3>(0));
                //g
                quad_to_sram.quad_color.g.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.quad_color.g.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.quad_color.g.set_slc(24, (inputChannel.read())<3>(0));
                //b
                quad_to_sram.quad_color.b.set_slc(0, (inputChannel.read())<12>(0));
                quad_to_sram.quad_color.b.set_slc(12, (inputChannel.read())<12>(0));
                quad_to_sram.quad_color.b.set_slc(24, (inputChannel.read())<3>(0));
                // end streaming for this quad definition
                buffer.data[i] = quad_to_sram;
                if (i == params.num_quads-1) break;
            }

            ac_int<11, false> spp;
            spp = (params.samp_per_pxl == 0) ? 32 :
                (params.samp_per_pxl == 1) ? 64 :
                (params.samp_per_pxl == 2) ? 256 : 1024;


            #define MAXLOOP 2748779069440
            ac_int<22, false> pixels_in_img = (params.image_height) * (params.image_width);
            ac_int<33, false> tot_samples_in_img = pixels_in_img * (spp);
            ac_int<36, false> tot_bounces = (tot_samples_in_img << 3) + 1; // multiply by 8 plus 1 for the extra end case bounce
            ac_int<43, false> tot_read_requests = (tot_bounces * (params.num_quads)); // multiply by 8
            // Reading from buffer multiple times based on image parameters
            #pragma hls_pipeline_init_interval 1
            for (ac_int<43,false> read_request = 0; read_request < MAXLOOP; read_request += 1){
                quads_out.write(buffer.data[i]);
                if (read_request == tot_read_requests-1) break;
            }
        }
    }
};

#endif

