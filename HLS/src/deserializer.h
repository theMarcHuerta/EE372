#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "RTcore.h"

class ParamsDeserializer{
  public:
    ParamsDeserializer(){}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ac_int<12,false>> &inputChannel,
                        ac_channel<buffer_params> &qbuffer_params,
                        ac_channel<img_params> &render_params,
                        ac_channel<img_params> &accum_params,
                        ac_channel<quad_hittable> &quad_serial_out
                        )
    {
        img_params image_params;
        // QUAD NUM //////////////////////////////////////////////////////
        image_params.num_quads.set_slc(0, (inputChannel.read()).slc<11>(0));
        // SAMPLES PER PIXEL /////////////////////////////////////////////
        image_params.samp_per_pxl.set_slc(0, (inputChannel.read()).slc<2>(0));  
        // BACKGORUND COLOR ///////////////////////////////////////////////
        // RED COLOR 27 bits 
        image_params.background.r.set_slc(0, (inputChannel.read()).slc<12>(0)); 
        image_params.background.r.set_slc(12, (inputChannel.read()).slc<12>(0)); 
        image_params.background.r.set_slc(24, (inputChannel.read()).slc<3>(0)); 
        // GREEN COLOR 27 bits 
        image_params.background.g.set_slc(0, (inputChannel.read()).slc<12>(0)); 
        image_params.background.g.set_slc(12, (inputChannel.read()).slc<12>(0)); 
        image_params.background.g.set_slc(24, (inputChannel.read()).slc<3>(0)); 
        // BLUE COLOR 27 bits 
        image_params.background.b.set_slc(0, (inputChannel.read()).slc<12>(0)); 
        image_params.background.b.set_slc(12, (inputChannel.read()).slc<12>(0)); 
        image_params.background.b.set_slc(24, (inputChannel.read()).slc<3>(0)); 
        // IMAGE HEIGHT AND WIDTH /////////////////////////////////////////
        image_params.image_height.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.image_height.set_slc(0, (inputChannel.read()).slc<11>(0));
        // CAMERA CENTER //////////////////////////////////////////////
        image_params.center.x.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.center.y.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.center.z.set_slc(0, (inputChannel.read()).slc<11>(0));
        // UPPER LEFT PIXEL 0,0 LOCATION /////////////////////////////////////
        image_params.pixel00_loc.x.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.x.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.x.set_slc(22, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.y.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.y.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.y.set_slc(22, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.z.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.z.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel00_loc.z.set_slc(22, (inputChannel.read()).slc<11>(0));
        // PIXEL DELTA U //////////////////////////////////////////////////////
        image_params.pixel_delta_u.x.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_u.x.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_u.x.set_slc(22, (inputChannel.read()).slc<3>(0));
        image_params.pixel_delta_u.y.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_u.y.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_u.y.set_slc(22, (inputChannel.read()).slc<3>(0));
        image_params.pixel_delta_u.z.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_u.z.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_u.z.set_slc(22, (inputChannel.read()).slc<3>(0));
        // PIXEL DELTA V //////////////////////////////////////////////////////
        image_params.pixel_delta_v.x.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_v.x.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_v.x.set_slc(22, (inputChannel.read()).slc<3>(0));
        image_params.pixel_delta_v.y.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_v.y.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_v.y.set_slc(22, (inputChannel.read()).slc<3>(0));
        image_params.pixel_delta_v.z.set_slc(0, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_v.z.set_slc(11, (inputChannel.read()).slc<11>(0));
        image_params.pixel_delta_v.z.set_slc(22, (inputChannel.read()).slc<3>(0));

        buffer_params params_to_buffer;
        params_to_buffer.image_height = image_params.image_height;
        params_to_buffer.image_width = image_params.image_width;
        params_to_buffer.num_quads = image_params.num_quads;
        params_to_buffer.samp_per_pxl = image_params.samp_per_pxl;

        qbuffer_params.write(params_to_buffer);  // why passing output channels
        render_params.write(image_params);
        accum_params.write(image_params);
        

        #pragma hls_pipeline_init_interval 1
        for (int i = 0; i < MAX_QUADS_IN_BUFFER-1; i++) {
            quad_hittable quad_to_sram;
            // CORNER POINT
            quad_to_sram.corner_pt.x.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.corner_pt.y.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.corner_pt.z.set_slc(0, (inputChannel.read()).slc<12>(0));
            // U VECTOR
            quad_to_sram.u.x.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.u.y.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.u.z.set_slc(0, (inputChannel.read()).slc<12>(0));
            // V VECTOR
            quad_to_sram.v.x.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.v.y.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.v.z.set_slc(0, (inputChannel.read()).slc<12>(0));
            // MATERIAL TYPE
            quad_to_sram.mat_type.set_slc(0, (inputChannel.read()).slc<3>(0));
            // IS INVISIBLE
            ac_int<1,false> isinvis;
            isinvis.set_slc(0, (inputChannel.read()).slc<1>(0));
            quad_to_sram.is_invis = isinvis ? true : false;
            // NORMAL VECTOR
            // x
            quad_to_sram.normal.x.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.normal.x.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.normal.x.set_slc(24, (inputChannel.read()).slc<2>(0));
            // y
            quad_to_sram.normal.y.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.normal.y.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.normal.y.set_slc(24, (inputChannel.read()).slc<2>(0));
            // z
            quad_to_sram.normal.z.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.normal.z.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.normal.z.set_slc(24, (inputChannel.read()).slc<2>(0));
            // W VECTOR
            //x
            quad_to_sram.w.x.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.w.x.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.w.x.set_slc(24, (inputChannel.read()).slc<1>(0));
            // y
            quad_to_sram.w.y.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.w.y.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.w.y.set_slc(24, (inputChannel.read()).slc<1>(0));
            // z
            quad_to_sram.w.z.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.w.z.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.w.z.set_slc(24, (inputChannel.read()).slc<1>(0));
            // D PLANE
            quad_to_sram.d_plane.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.d_plane.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.d_plane.set_slc(24, (inputChannel.read()).slc<7>(0));
            // QUAD COLOR
            //r
            quad_to_sram.quad_color.r.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.quad_color.r.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.quad_color.r.set_slc(24, (inputChannel.read()).slc<3>(0));
            //g
            quad_to_sram.quad_color.g.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.quad_color.g.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.quad_color.g.set_slc(24, (inputChannel.read()).slc<3>(0));
            //b
            quad_to_sram.quad_color.b.set_slc(0, (inputChannel.read()).slc<12>(0));
            quad_to_sram.quad_color.b.set_slc(12, (inputChannel.read()).slc<12>(0));
            quad_to_sram.quad_color.b.set_slc(24, (inputChannel.read()).slc<3>(0));
            quad_serial_out.write(quad_to_sram);
            if (i == params_to_buffer.num_quads-1) break; 
        }

    }

  private:
    ac_int<12,false> read_in_data;

};

#endif
