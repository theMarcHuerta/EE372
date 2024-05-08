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
                    ac_channel<ac_int<12,false>> &quad_serial_out
                    )
    {
        img_params image_params;
        // QUAD NUM //////////////////////////////////////////////////////
        image_params.num_quads.set_slc(0, (inputChannel.read())<11>(0));
        // SAMPLES PER PIXEL /////////////////////////////////////////////
        image_params.samp_per_pxl.set_slc(0, (inputChannel.read())<2>(0));  
        // BACKGORUND COLOR ///////////////////////////////////////////////
        // RED COLOR 27 bits 
        image_params.background.r.set_slc(0, (inputChannel.read())<12>(0)); 
        image_params.background.r.set_slc(12, (inputChannel.read())<12>(0)); 
        image_params.background.r.set_slc(24, (inputChannel.read())<3>(0)); 
        // GREEN COLOR 27 bits 
        image_params.background.g.set_slc(0, (inputChannel.read())<12>(0)); 
        image_params.background.g.set_slc(12, (inputChannel.read())<12>(0)); 
        image_params.background.g.set_slc(24, (inputChannel.read())<3>(0)); 
        // BLUE COLOR 27 bits 
        image_params.background.b.set_slc(0, (inputChannel.read())<12>(0)); 
        image_params.background.b.set_slc(12, (inputChannel.read())<12>(0)); 
        image_params.background.b.set_slc(24, (inputChannel.read())<3>(0)); 
        // IMAGE HEIGHT AND WIDTH /////////////////////////////////////////
        image_params.image_height.set_slc(0, (inputChannel.read())<11>(0));
        image_params.image_height.set_slc(0, (inputChannel.read())<11>(0));
        // CAMERA CENTER //////////////////////////////////////////////
        image_params.center.x.set_slc(0, (inputChannel.read())<11>(0));
        image_params.center.y.set_slc(0, (inputChannel.read())<11>(0));
        image_params.center.z.set_slc(0, (inputChannel.read())<11>(0));
        // UPPER LEFT PIXEL 0,0 LOCATION /////////////////////////////////////
        image_params.pixel00_loc.x.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.x.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.x.set_slc(22, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.y.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.y.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.y.set_slc(22, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.z.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.z.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel00_loc.z.set_slc(22, (inputChannel.read())<11>(0));
        // PIXEL DELTA U //////////////////////////////////////////////////////
        image_params.pixel_delta_u.x.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel_delta_u.x.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel_delta_u.x.set_slc(22, (inputChannel.read())<3>(0));
        image_params.pixel_delta_u.y.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel_delta_u.y.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel_delta_u.y.set_slc(22, (inputChannel.read())<3>(0));
        image_params.pixel_delta_u.z.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel_delta_u.z.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel_delta_u.z.set_slc(22, (inputChannel.read())<3>(0))
        // PIXEL DELTA V //////////////////////////////////////////////////////
        image_params.pixel_delta_v.x.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel_delta_v.x.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel_delta_v.x.set_slc(22, (inputChannel.read())<3>(0));
        image_params.pixel_delta_v.y.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel_delta_v.y.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel_delta_v.y.set_slc(22, (inputChannel.read())<3>(0));
        image_params.pixel_delta_v.z.set_slc(0, (inputChannel.read())<11>(0));
        image_params.pixel_delta_v.z.set_slc(11, (inputChannel.read())<11>(0));
        image_params.pixel_delta_v.z.set_slc(22, (inputChannel.read())<3>(0));

        buffer_params params_to_buffer;
        params_to_buffer.image_height = image_params.image_height;
        params_to_buffer.image_width = image_params.image_width;
        params_to_buffer.num_quads = image_params.num_quads;
        params_to_buffer.samp_per_pxl = image_params.samp_per_pxl;

        qbuffer_params.write(params_to_buffer);  // why passing output channels
        render_params.write(image_params);
        accum_params.write(image_params);

        #ifndef __SYNTHESIS__
        while(inputChannel.available(1))
        #endif
        {
            quad_serial_out.write(inputChannel.read());
        }
    }

};

#endif
