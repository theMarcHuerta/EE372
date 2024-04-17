#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "rtccore.h"

template<typename DTYPE_SERIAL, typename DTYPE, int n>
class Deserializer{
public:
    Deserializer(){}

#pragma hls_design interface
void CCS_BLOCK(run)(ac_channel<DTYPE_SERIAL> &inputChannel,
                    ac_channel<DTYPE> &outputChannel)
    {
        #ifndef __SYNTHESIS__
        while(inputChannel.available(1))
        #endif
        {
            DTYPE output;
            for(int i = 0; i < n; i++){
                output.value[i] = inputChannel.read();
            }
            outputChannel.write(output);

        }
    }
};

class ParamsDeserializer{
public:
    ParamsDeserializer(){}

#pragma hls_design interface
void CCS_BLOCK(run)(ac_channel<int_11> &inputChannel1,
                    ac_channel<Params> &outputChannel1,
                    ac_channel<Params> &outputChannel2,
                    ac_channel<Params> &outputChannel3,
                    ac_channel<Params> &outputChannel4
                    )
    {
        img_params image_params;
        // chan 1
        image_params.num_spheres = inputChannel.read();
        image_params.num_quads = inputChannel.read(); 
        image_params.samp_per_pxl = inputChannel.read();  //trunc
        image_params.background.r = inputChannel.read(); //trunc
        image_params.background.g = inputChannel.read(); //trunc
        image_params.background.b = inputChannel.read(); //trunc
        image_params.image_height = inputChannel.read();
        image_params.image_height = inputChannel.read();
        image_params.center.x = inputChannel.read();
        image_params.center.y = inputChannel.read();
        image_params.center.z = inputChannel.read();
        image_params.pixel00_loc.x_i = inputChannel.read();
        image_params.pixel00_loc.x_f1 = inputChannel.read();
        image_params.pixel00_loc.x_f2 = inputChannel.read();
        image_params.pixel00_loc.y_i = inputChannel.read();
        image_params.pixel00_loc.y_f1 = inputChannel.read();
        image_params.pixel00_loc.y_f2 = inputChannel.read();
        image_params.pixel00_loc.z_i = inputChannel.read();
        image_params.pixel00_loc.z_f1 = inputChannel.read();
        image_params.pixel00_loc.z_f2 = inputChannel.read();
        image_params.pixel_delta_u.x_i = inputChannel.read();
        image_params.pixel_delta_u.x_f1 = inputChannel.read();
        image_params.pixel_delta_u.x_f2 = inputChannel.read();
        image_params.pixel_delta_u.y_i = inputChannel.read();
        image_params.pixel_delta_u.y_f1 = inputChannel.read();
        image_params.pixel_delta_u.y_f2 = inputChannel.read();
        image_params.pixel_delta_u.z_i = inputChannel.read();
        image_params.pixel_delta_u.z_f1 = inputChannel.read();
        image_params.pixel_delta_u.z_f2 = inputChannel.read();
        image_params.pixel_delta_v.x_i = inputChannel.read();
        image_params.pixel_delta_v.x_f1 = inputChannel.read();
        image_params.pixel_delta_v.x_f2 = inputChannel.read();
        image_params.pixel_delta_v.y_i = inputChannel.read();
        image_params.pixel_delta_v.y_f1 = inputChannel.read();
        image_params.pixel_delta_v.y_f2 = inputChannel.read();
        image_params.pixel_delta_v.z_i = inputChannel.read();
        image_params.pixel_delta_v.z_f1 = inputChannel.read();
        image_params.pixel_delta_v.z_f2 = inputChannel.read();


        outputChannel1.write(params);  // why passing output channels
        outputChannel2.write(params);
        outputChannel3.write(image_params);

        for (int i = 0; i < ; i++) {
            outputChannel4.write(params);
        }
    }

};

#endif
