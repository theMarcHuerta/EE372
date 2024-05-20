#ifndef QUADSBUFFER_H
#define QUADSBUFFER_H

#include "RTcore.h"

template <int size = MAX_QUADS_IN_BUFFER>
class QuadBuffer {
public:
    QuadBuffer() {
        qin_reg = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, {0, 0, 0}, {0, 0, 0}, 0, {0, 0, 0}};
        qout_reg = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, {0, 0, 0}, {0, 0, 0}, 0, {0, 0, 0}};
    }

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

            uint_11 stop_point_height = params.image_end - params.image_start -1;
            uint_11 stop_point_width= params.image_width-1;
            
            // Quad buffer is made up of 12 banks
            chanStruct<ac_int<32, false>, size> buffer_bank0;
            chanStruct<ac_int<32, false>, size> buffer_bank1;
            chanStruct<ac_int<32, false>, size> buffer_bank2;
            chanStruct<ac_int<32, false>, size> buffer_bank3;
            chanStruct<ac_int<32, false>, size> buffer_bank4;
            chanStruct<ac_int<32, false>, size> buffer_bank5;
            chanStruct<ac_int<32, false>, size> buffer_bank6;
            chanStruct<ac_int<32, false>, size> buffer_bank7;
            chanStruct<ac_int<32, false>, size> buffer_bank8;
            chanStruct<ac_int<32, false>, size> buffer_bank9;
            chanStruct<ac_int<32, false>, size> buffer_bank10;
            chanStruct<ac_int<32, false>, size> buffer_bank11;
            
            // Writing to buffer
            // #pragma hls_pipeline_init_interval 1
            for (int i = 0; i < MAX_QUADS_IN_BUFFER; i++) {
                qin_reg = quads_in.read();

                // set one bit data type for is_invis based on boolean value for slicing
                pbool inv = (qin_reg.is_invis) ? 1 : 0;

                // write to buffer
                buffer_bank0.data[i].set_slc(0, qin_reg.corner_pt.x.slc<12>(0));
                buffer_bank0.data[i].set_slc(12, qin_reg.corner_pt.y.slc<12>(0));
                buffer_bank0.data[i].set_slc(24, qin_reg.corner_pt.z.slc<8>(0));

                buffer_bank1.data[i].set_slc(0, qin_reg.corner_pt.z.slc<4>(8));
                buffer_bank1.data[i].set_slc(4, qin_reg.u.x.slc<12>(0));
                buffer_bank1.data[i].set_slc(16, qin_reg.u.y.slc<12>(0));
                buffer_bank1.data[i].set_slc(28, qin_reg.u.z.slc<4>(0));

                buffer_bank2.data[i].set_slc(0, qin_reg.u.z.slc<8>(4));
                buffer_bank2.data[i].set_slc(8, qin_reg.v.x.slc<12>(0));
                buffer_bank2.data[i].set_slc(20, qin_reg.v.y.slc<12>(0));

                buffer_bank3.data[i].set_slc(0, qin_reg.v.z.slc<12>(0));
                buffer_bank3.data[i].set_slc(12, qin_reg.mat_type.slc<3>(0));
                buffer_bank3.data[i].set_slc(15, inv.slc<1>(0));
                buffer_bank3.data[i].set_slc(16, qin_reg.normal.x.slc<16>(0));

                buffer_bank4.data[i].set_slc(0, qin_reg.normal.x.slc<10>(16));
                buffer_bank4.data[i].set_slc(10, qin_reg.normal.y.slc<22>(0));

                buffer_bank5.data[i].set_slc(0, qin_reg.normal.y.slc<4>(22));
                buffer_bank5.data[i].set_slc(4, qin_reg.normal.z.slc<26>(0));
                buffer_bank5.data[i].set_slc(30, qin_reg.w.x.slc<2>(0));

                buffer_bank6.data[i].set_slc(0, qin_reg.w.x.slc<23>(2));
                buffer_bank6.data[i].set_slc(23, qin_reg.w.y.slc<9>(0));

                buffer_bank7.data[i].set_slc(0, qin_reg.w.y.slc<16>(9));
                buffer_bank7.data[i].set_slc(16, qin_reg.w.z.slc<16>(0));

                buffer_bank8.data[i].set_slc(0, qin_reg.w.z.slc<9>(16));
                buffer_bank8.data[i].set_slc(9, qin_reg.d_plane.slc<23>(0));

                buffer_bank9.data[i].set_slc(0, qin_reg.d_plane.slc<8>(23));
                buffer_bank9.data[i].set_slc(8, qin_reg.quad_color.r.slc<24>(0));

                buffer_bank10.data[i].set_slc(0, qin_reg.quad_color.r.slc<3>(24));
                buffer_bank10.data[i].set_slc(3, qin_reg.quad_color.g.slc<27>(0));
                buffer_bank10.data[i].set_slc(30, qin_reg.quad_color.b.slc<2>(0));

                buffer_bank11.data[i].set_slc(0, qin_reg.quad_color.b.slc<25>(2));
                

                if (i == params.num_quads-1) break; // check if its odd or even
            }


            ac_int<11, false> spp;
            spp = (params.samp_per_pxl == 0) ? 32 :
                (params.samp_per_pxl == 1) ? 64 :
                (params.samp_per_pxl == 2) ? 256 : 1024;



            #define MAXLOOP 34359738369
            ac_int<22, false> pixels_in_img = (params.image_end - params.image_start) * (params.image_width);
            ac_int<33, false> tot_samples_in_img = pixels_in_img * (spp);
            ac_int<36, false> tot_read_requests = (tot_samples_in_img << 3) + 2; // multiply by 8 plus 1 for the extra end case bounce
            // Reading from buffer multiple times based on image parameters
            #pragma hls_pipeline_init_interval 1
            for (ac_int<43,false> read_request = 0; read_request < MAXLOOP; read_request += 1){
                for (ac_int<7, false> quad_idx = 0; quad_idx < MAX_QUADS_IN_BUFFER; quad_idx += 1) {

                    ac_int<32, false> slice0 = buffer_bank0.data[quad_idx];
                    ac_int<32, false> slice1 = buffer_bank1.data[quad_idx];
                    ac_int<32, false> slice2 = buffer_bank2.data[quad_idx];
                    ac_int<32, false> slice3 = buffer_bank3.data[quad_idx];
                    ac_int<32, false> slice4 = buffer_bank4.data[quad_idx];
                    ac_int<32, false> slice5 = buffer_bank5.data[quad_idx];
                    ac_int<32, false> slice6 = buffer_bank6.data[quad_idx];
                    ac_int<32, false> slice7 = buffer_bank7.data[quad_idx];
                    ac_int<32, false> slice8 = buffer_bank8.data[quad_idx];
                    ac_int<32, false> slice9 = buffer_bank9.data[quad_idx];
                    ac_int<32, false> slice10 = buffer_bank10.data[quad_idx];
                    ac_int<32, false> slice11 = buffer_bank11.data[quad_idx];

                    qout_reg.corner_pt.x.set_slc(0, slice0.slc<12>(0));
                    qout_reg.corner_pt.y.set_slc(0, slice0.slc<12>(12));
                    qout_reg.corner_pt.z.set_slc(0, slice0.slc<8>(24));

                    qout_reg.corner_pt.z.set_slc(8, slice1.slc<4>(0));
                    qout_reg.u.x.set_slc(0, slice1.slc<12>(4));
                    qout_reg.u.y.set_slc(0, slice1.slc<12>(16));
                    qout_reg.u.z.set_slc(0, slice1.slc<4>(28));

                    qout_reg.u.z.set_slc(4, slice2.slc<8>(0));
                    qout_reg.v.x.set_slc(0, slice2.slc<12>(8));
                    qout_reg.v.y.set_slc(0, slice2.slc<12>(20));

                    qout_reg.v.z.set_slc(0, slice3.slc<12>(0));
                    qout_reg.mat_type.set_slc(0, slice3.slc<3>(12));
                    qout_reg.is_invis = (slice3[15] == 1) ? true : false;
                    qout_reg.normal.x.set_slc(0, slice3.slc<16>(16));

                    qout_reg.normal.x.set_slc(16, slice4.slc<10>(0));
                    qout_reg.normal.y.set_slc(0, slice4.slc<22>(10));

                    qout_reg.normal.y.set_slc(22, slice5.slc<4>(0));
                    qout_reg.normal.z.set_slc(0, slice5.slc<26>(4));
                    qout_reg.w.x.set_slc(0, slice5.slc<2>(30));

                    qout_reg.w.x.set_slc(2, slice6.slc<23>(0));
                    qout_reg.w.y.set_slc(0, slice6.slc<9>(23));

                    qout_reg.w.y.set_slc(9, slice7.slc<16>(0));
                    qout_reg.w.z.set_slc(0, slice7.slc<16>(16));

                    qout_reg.w.z.set_slc(16, slice8.slc<9>(0));
                    qout_reg.d_plane.set_slc(0, slice8.slc<23>(9));

                    qout_reg.d_plane.set_slc(23, slice9.slc<8>(0));
                    qout_reg.quad_color.r.set_slc(0, slice9.slc<24>(8));

                    qout_reg.quad_color.r.set_slc(24, slice10.slc<3>(0));
                    qout_reg.quad_color.g.set_slc(0, slice10.slc<27>(3));
                    qout_reg.quad_color.b.set_slc(0, slice10.slc<2>(30));

                    qout_reg.quad_color.b.set_slc(2, slice11.slc<25>(0));

                    quads_out.write(qout_reg);

                    if (quad_idx == params.num_quads-1) break;
                }
                if (read_request == tot_read_requests-1) break;
            }
        }
    }

private:
    quad_hittable qin_reg;
    quad_hittable qout_reg;

};

#endif

