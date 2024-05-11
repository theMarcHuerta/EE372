#ifndef SHADER_CORES_H
#define SHADER_CORES_H

#include "RTcore.h"
#include "Shader.h"
#include "ShaderFeedbackController.h"

class ShaderCores
{
public:
    ShaderCores() : accumalated_color_chan2(1), attenuation_chan2(1), ray_out1(1){
    }
    
<<<<<<< HEAD
#pragma hls_design interface
// #pragma hls_pipeline_init_interval 10
=======
    #pragma hls_design interface
>>>>>>> 6b35beecc7aef5f5649b34672c794f19a1d64f51
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
             ac_channel<ray> &ray_in,
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<rgb_in> &output_pxl_serial
            )
    {


    #ifndef __SYNTHESIS__
    while(params_in.available(1))
    #endif
    {
      // #ifndef __SYNTHESIS__
      // // if (iter % 320 == 0)
      //   std::cout << "in loop " << std::endl;
      // #endif
      // controller.run(ray_in, ray_out1, params_in, accumalated_color_chan2, attenuation_chan2, 
      //                 ray_into_shader, params_to_shader, accumalated_color_chan1, attenuation_chan1, output_pxl_serial);

      controller.run(ray_in, ray_out1, params_in, accumalated_color_chan2, attenuation_chan2,
                ray_into_shader, params_to_shader, accumalated_color_chan1, attenuation_chan1, output_pxl_serial);

      // shader1.run(params_to_shader, quads_in, ray_into_shader, attenuation_chan1, accumalated_color_chan1, 
      //            accumalated_color_chan2);
      shader1.run(params_to_shader, quads_in, ray_into_shader, attenuation_chan1, accumalated_color_chan1, 
                  attenuation_chan2, accumalated_color_chan2, ray_out1);
    }
  }
private:
  // const ray init_ray = {{0,0,0}, {0,0,0}, false};
  // static const rgb_in init_col = {0,0,0};

        // ac_channel<ray> ray_out1(2,init_ray);
        // ac_channel<rgb_in> accumalated_color_chan2(2,init_col);
        // ac_channel<rgb_in> attenuation_chan2(2,init_col);

        // ac_channel<ray> ray_out1(2, ray(({0,0,0}, {0,0,0}, false)));
        // ac_channel<rgb_in> accumalated_color_chan2(2, rgb_in(0));
        // ac_channel<rgb_in> attenuation_chan2(2, rgb_in(0));

        
  ac_channel<ray> ray_out1;
  ac_channel<rgb_in> accumalated_color_chan2;
  ac_channel<rgb_in> attenuation_chan2;

  Shader shader1;
  ShaderFeedbackController controller;

  // ac_channel<bool> last;

  ac_channel<ray> ray_into_shader;
  // ac_channel<ray> ray_out1(1,{{0,0,0}, {0,0,0}, false});
  ac_channel<ray> ray_from_shader;

  ac_channel<buffer_obj_count> params_to_shader;
  ac_channel<buffer_obj_count> params_to_cntrlr;

  ac_channel<rgb_in> accumalated_color_chan1;
  // ac_channel<rgb_in> accumalated_color_chan2(1,{0,0,0});
  ac_channel<rgb_in> color_from_shader;

  ac_channel<rgb_in> attenuation_chan1;
  // ac_channel<rgb_in> attenuation_chan2(1,{0,0,0});
  ac_channel<rgb_in> atten_from_shader;

};

#endif
