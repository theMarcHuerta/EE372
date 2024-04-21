#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"
#include "MemController.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class ShaderCores
{
public:
    ShaderCores(){}
    
#pragma hls_design interface
    void run(ac_channel<sphere_hittable> &spheres_in,
             ac_channel<quad_hittable> &quads_in, 
             ac_channel<ray<sfp_11_22>> &ray_in,
             ac_channel<LoopIndices> &loop_in,
             ac_channel<img_params> &params_in,
             ac_channel<rgb_t<sfp_9_10>> &output_pxl_serial
            )
    {
      // pass in all variables to loop 
      // should be the ray and access to the buffer/ram holding the sphere and quad hittables/primitaives
      // this should return a stream of outputted pixels / vec3 of 8 bits
      // DO WE HAVE FIFO OR WHAT
      img_params tmp_params;
        //outer loop
      tmp_params = render_params.read();
      for (int fy = 0; fy < tmp_params.image_height; fy++){
        for (int fx = 0; fx < tmp_params.image_width; fx++){
          for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
            rgb_t<sfp_9_10> shader1_atten;
            rgb_t<sfp_9_10> shader1_color;
            shader1_color.r = 0;
            shader1_color.g = 0;
            shader1_color.b = 0;

            shader1_atten.r = MAX_RGB_INT_VALUE;
            shader1_atten.g = MAX_RGB_INT_VALUE;
            shader1_atten.b = MAX_RGB_INT_VALUE;
            // TO DO: CHANGE RBG VALUE SIZING TO ADJUST FOR BIT SIZES IN ACUMULATED COLOR OUTPUT TO AVOID OVERFLOW
            // for the first shader-- this will just be the default starting point
            // can also just make the shader cores templated or unique for the first and last ones so they dont waste i/o
            // not sure if the unused i/o gets optimized out so we can do that later in the optimization phase
            attenuation_chan[0].write(shader1_atten);
            attenuation_chan[0].write(shader1_atten);

            memController.run();
            shader1.run(sphere_in[0], quad_in[0], ray_in, attenuation_chan[0], accumalated_color_chan[0], attenuation_chan[1], accumalated_color_chan[1], ray_out[0]);
            shader2.run(sphere_in[1], quad_in[1], ray_out[0], attenuation_chan[1], accumalated_color_chan[1], attenuation_chan[2], accumalated_color_chan[2], ray_out[1]);
            shader3.run(sphere_in[2], quad_in[2], ray_out[1], attenuation_chan[2], accumalated_color_chan[2], attenuation_chan[3], accumalated_color_chan[3], ray_out[2]);
            shader4.run(sphere_in[3], quad_in[3], ray_out[2], attenuation_chan[3], accumalated_color_chan[3], attenuation_chan[4], accumalated_color_chan[4], ray_out[3]);
            shader5.run(sphere_in[4], quad_in[4], ray_out[3], attenuation_chan[4], accumalated_color_chan[4], attenuation_chan[5], accumalated_color_chan[5], ray_out[4]);
            shader6.run(sphere_in[5], quad_in[5], ray_out[4], attenuation_chan[5], accumalated_color_chan[5], attenuation_chan[6], accumalated_color_chan[6], ray_out[5]);
            shader7.run(sphere_in[6], quad_in[6], ray_out[5], attenuation_chan[6], accumalated_color_chan[6], attenuation_chan[7], accumalated_color_chan[7], ray_out[6]);
            shader8.run(sphere_in[7], quad_in[7], ray_out[6], attenuation_chan[7], accumalated_color_chan[7], attenuation_chan[8], pxl_color_out, ray_out[7]);
          }
        }
      }
    }
private:
  Shader shader1;
  Shader shader2;
  Shader shader3;
  Shader shader4;
  Shader shader5;
  Shader shader6;
  Shader shader7;
  Shader shader8;
  MemController memController;

  ac_channel<ray<sfp_11_22>> ray_out[8];
  ac_channel<rgb_t<sfp_9_10>> accumalated_color_chan[8];
  ac_channel<rgb_t<sfp_9_10>> attenuation_chan[9];
  ac_channel<sphere_hittable> sphere_in[8];
  ac_channel<quad_hittable> quad_in[8];
};

