#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"
#include "MemController.h"
#include "Shader.h"
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
             ac_channel<rgb_in> &output_pxl_serial
            )
    {
    #ifndef __SYNTHESIS__
    while(params_in.available(1))
    #endif
    {
      // pass in all variables to loop 
      // should be the ray and access to the buffer/ram holding the sphere and quad hittables/primitaives
      // this should return a stream of outputted pixels / vec3 of 8 bits
      // DO WE HAVE FIFO OR WHAT
      img_params tmp_params;
        //outer loop
      tmp_params = params_in.read();
      rgb_in shader1_atten = {1, 1, 1};
      rgb_in shader1_color = {0, 0, 0};

      // TO DO: CHANGE RBG VALUE SIZING TO ADJUST FOR BIT SIZES IN ACUMULATED COLOR OUTPUT TO AVOID OVERFLOW
      // for the first shader-- this will just be the default starting point
      // can also just make the shader cores templated or unique for the first and last ones so they dont waste i/o
      // not sure if the unused i/o gets optimized out so we can do that later in the optimization phase
      attenuation_chan[0].write(shader1_atten);
      accumalated_color_chan[0].write(shader1_color);

      memController.run();
      shader1.run(sphere_in[0], quad_in[0], ray_in, attenuation_chan[0], accumalated_color_chan[0], attenuation_chan[1], accumalated_color_chan[1], ray_out[0]);
      shader2.run(sphere_in[1], quad_in[1], ray_out[0], attenuation_chan[1], accumalated_color_chan[1], attenuation_chan[2], accumalated_color_chan[2], ray_out[1]);
      shader3.run(sphere_in[2], quad_in[2], ray_out[1], attenuation_chan[2], accumalated_color_chan[2], attenuation_chan[3], accumalated_color_chan[3], ray_out[2]);
      shader4.run(sphere_in[3], quad_in[3], ray_out[2], attenuation_chan[3], accumalated_color_chan[3], attenuation_chan[4], accumalated_color_chan[4], ray_out[3]);
      shader5.run(sphere_in[4], quad_in[4], ray_out[3], attenuation_chan[4], accumalated_color_chan[4], attenuation_chan[5], accumalated_color_chan[5], ray_out[4]);
      shader6.run(sphere_in[5], quad_in[5], ray_out[4], attenuation_chan[5], accumalated_color_chan[5], attenuation_chan[6], accumalated_color_chan[6], ray_out[5]);
      shader7.run(sphere_in[6], quad_in[6], ray_out[5], attenuation_chan[6], accumalated_color_chan[6], attenuation_chan[7], accumalated_color_chan[7], ray_out[6]);
      shader8.run(sphere_in[7], quad_in[7], ray_out[6], attenuation_chan[7], accumalated_color_chan[7], attenuation_chan[8], output_pxl_serial, ray_out[7]);
    }
  }
private:
  Shader<sfp_11_22, fp_1_22> shader1;
  Shader<sfp_11_22, fp_1_22> shader2;
  Shader<sfp_11_22, fp_1_22> shader3;
  Shader<sfp_11_22, fp_1_22> shader4;
  Shader<sfp_11_22, fp_1_22> shader5;
  Shader<sfp_11_22, fp_1_22> shader6;
  Shader<sfp_11_22, fp_1_22> shader7;
  Shader<sfp_11_22, fp_1_22> shader8;
  MemController memController;

  ac_channel<ray<sfp_11_22>> ray_out[8];
  ac_channel<rgb_in> accumalated_color_chan[8];
  ac_channel<rgb_in> attenuation_chan[9];
  ac_channel<sphere_hittable> sphere_in[8];
  ac_channel<quad_hittable> quad_in[8];
};

