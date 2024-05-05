#ifndef SHADER_CORES_H
#define SHADER_CORES_H

#include "RTcore.h"
#include "MemController.h"
#include "Shader.h"


class ShaderCores
{
public:
    ShaderCores(){}
    
#pragma hls_design interface
#pragma hls_pipeline_init_interval 1
    void run(ac_channel<quad_hittable> &quads_in, 
             ac_channel<ray> &ray_in,
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<rgb_in> &output_pxl_serial
            )
    {
    #ifndef __SYNTHESIS__
    while(params_in.available(1))
    #endif
    {

      attenuation_chan1.write(shader1_atten);
      accumalated_color_chan1.write(shader1_color);

      buffer_obj_count tmp_params = params_in.read();

      // in 1 loop iteration of this, we want to read the buffer 4x and get the quads out to the shader cores 4x
      for (int i = 0; i < 8; i++){

        params_to_shader.write(tmp_params);

        if (i == 0){
          // only read input ray 1x
          ray_into_shader.write(ray_in.read());
        }
        else{
          // will make sure shader 1 gets it's old output 
          ray_into_shader.write(ray_out1.read());
        }

        shader1.run(params_to_shader, quads_in, ray_into_shader, attenuation_chan1, accumalated_color_chan1, attenuation_chan2, accumalated_color_chan2, ray_out1);

        // makes sure this excutes after first 2 bounces
        if (i != 7){
          // INFO FROM SHADER 1 GO BACK TO SHADER 1
          attenuation_chan1.write(attenuation_chan2.read());
          accumalated_color_chan1.write(accumalated_color_chan2.read());
        }
        else {
          // from shader too after x bounces, then we have our output pixel
          output_pxl_serial.write(accumalated_color_chan2.read());
          ray_out1.read(); // read to make sure it doesnt get jammed? not sure how it works but this goes nowehere
          attenuation_chan2.read();
        }
      }
    }
  }
private:

  const rgb_in shader1_atten = {1, 1, 1};
  const rgb_in shader1_color = {0, 0, 0};

  ac_channel<ray> ray_into_shader;
  ac_channel<ray> ray_in1;

  Shader shader1;

  ac_channel<buffer_obj_count> params_to_shader;

  ac_channel<ray> ray_out1;

  ac_channel<rgb_in> accumalated_color_chan1;
  ac_channel<rgb_in> accumalated_color_chan2;

  ac_channel<rgb_in> attenuation_chan1;
  ac_channel<rgb_in> attenuation_chan2;

};

#endif
