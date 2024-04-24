#ifndef SHADER_CORES_H
#define SHADER_CORES_H

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
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<rgb_in> &output_pxl_serial
            )
    {
    #ifndef __SYNTHESIS__
    while(params_in.available(1))
    #endif
    {

      // TO DO: CHANGE RBG VALUE SIZING TO ADJUST FOR BIT SIZES IN ACUMULATED COLOR OUTPUT TO AVOID OVERFLOW
      // for the first shader-- this will just be the default starting point
      // can also just make the shader cores templated or unique for the first and last ones so they dont waste i/o
      // not sure if the unused i/o gets optimized out so we can do that later in the optimization phase
      memController.run(spheres_in, quads_in, ray_in, params_in, sphere_in1, sphere_in2, sphere_in3, sphere_in4, sphere_in5, sphere_in6, sphere_in7, sphere_in8,
                        quad_in1, quad_in2, quad_in3, quad_in4, quad_in5, quad_in6, quad_in7, quad_in8, params_out_mem_cntrlr, ray_in_from_cntrlr);

      buffer_params params_to_cores;
      params_to_cores = params_out_mem_cntrlr.read();

      params_c1.write(params_to_cores);
      params_c2.write(params_to_cores);
      params_c3.write(params_to_cores);
      params_c4.write(params_to_cores);
      params_c5.write(params_to_cores);
      params_c6.write(params_to_cores);
      params_c7.write(params_to_cores);
      params_c8.write(params_to_cores);

      attenuation_chan1.write(shader1_atten);
      accumalated_color_chan1.write(shader1_color);
      shader1.run(params_c1, sphere_in1, quad_in1, ray_in_from_cntrlr, attenuation_chan1, accumalated_color_chan1, attenuation_chan2, accumalated_color_chan2, ray_out1);
      shader2.run(params_c2, sphere_in2, quad_in2, ray_out1, attenuation_chan2, accumalated_color_chan2, attenuation_chan3, accumalated_color_chan3, ray_out2);
      shader3.run(params_c3, sphere_in3, quad_in3, ray_out2, attenuation_chan3, accumalated_color_chan3, attenuation_chan4, accumalated_color_chan5, ray_out3);
      shader4.run(params_c4, sphere_in4, quad_in4, ray_out3, attenuation_chan4, accumalated_color_chan4, attenuation_chan5, accumalated_color_chan6, ray_out4);
      shader5.run(params_c5, sphere_in5, quad_in5, ray_out4, attenuation_chan5, accumalated_color_chan5, attenuation_chan6, accumalated_color_chan7, ray_out5);
      shader6.run(params_c6, sphere_in6, quad_in6, ray_out5, attenuation_chan6, accumalated_color_chan6, attenuation_chan7, accumalated_color_chan8, ray_out6);
      shader7.run(params_c7, sphere_in7, quad_in7, ray_out6, attenuation_chan7, accumalated_color_chan7, attenuation_chan8, accumalated_color_chan8, ray_out7);
      shader8.run(params_c8, sphere_in8, quad_in8, ray_out7, attenuation_chan7, accumalated_color_chan8, attenuation_chan9, output_pxl_serial, ray_out8);
    }
  }
private:

  const rgb_in shader1_atten = {1, 1, 1};
  const rgb_in shader1_color = {0, 0, 0};

  ac_channel<ray<sfp_11_22>> ray_in_from_cntrlr;

  ac_channel<sphere_hittable> sphere_out1;
  ac_channel<sphere_hittable> sphere_out2;
  ac_channel<sphere_hittable> sphere_out3;
  ac_channel<sphere_hittable> sphere_out4;
  ac_channel<sphere_hittable> sphere_out5;
  ac_channel<sphere_hittable> sphere_out6;
  ac_channel<sphere_hittable> sphere_out7;
  ac_channel<sphere_hittable> sphere_out8;
  ac_channel<quad_hittable> quad_out1;
  ac_channel<quad_hittable> quad_out2; 
  ac_channel<quad_hittable> quad_out3; 
  ac_channel<quad_hittable> quad_out4; 
  ac_channel<quad_hittable> quad_out5; 
  ac_channel<quad_hittable> quad_out6; 
  ac_channel<quad_hittable> quad_out7; 
  ac_channel<quad_hittable> quad_out8; 
  ac_channel<buffer_obj_count> params_out;
  ac_channel<ray<sfp_11_22>> ray_out;


  Shader<sfp_11_22, fp_1_22> shader1;
  Shader<sfp_11_22, fp_1_22> shader2;
  Shader<sfp_11_22, fp_1_22> shader3;
  Shader<sfp_11_22, fp_1_22> shader4;
  Shader<sfp_11_22, fp_1_22> shader5;
  Shader<sfp_11_22, fp_1_22> shader6;
  Shader<sfp_11_22, fp_1_22> shader7;
  Shader<sfp_11_22, fp_1_22> shader8;
  MemController memController;

  ac_channel<buffer_obj_count> params_out_mem_cntrlr;
  ac_channel<buffer_obj_count> params_c1;
  ac_channel<buffer_obj_count> params_c2;
  ac_channel<buffer_obj_count> params_c3;
  ac_channel<buffer_obj_count> params_c4;
  ac_channel<buffer_obj_count> params_c5;
  ac_channel<buffer_obj_count> params_c6;
  ac_channel<buffer_obj_count> params_c7;
  ac_channel<buffer_obj_count> params_c8;

  ac_channel<ray<sfp_11_22>> ray_out1;
  ac_channel<ray<sfp_11_22>> ray_out2;
  ac_channel<ray<sfp_11_22>> ray_out3;
  ac_channel<ray<sfp_11_22>> ray_out4;
  ac_channel<ray<sfp_11_22>> ray_out5;
  ac_channel<ray<sfp_11_22>> ray_out6;
  ac_channel<ray<sfp_11_22>> ray_out7;
  ac_channel<ray<sfp_11_22>> ray_out8;

  ac_channel<rgb_in> accumalated_color_chan1;
  ac_channel<rgb_in> accumalated_color_chan2;
  ac_channel<rgb_in> accumalated_color_chan3;
  ac_channel<rgb_in> accumalated_color_chan4;
  ac_channel<rgb_in> accumalated_color_chan5;
  ac_channel<rgb_in> accumalated_color_chan6;
  ac_channel<rgb_in> accumalated_color_chan7;
  ac_channel<rgb_in> accumalated_color_chan8;

  ac_channel<rgb_in> attenuation_chan1;
  ac_channel<rgb_in> attenuation_chan2;
  ac_channel<rgb_in> attenuation_chan3;
  ac_channel<rgb_in> attenuation_chan4;
  ac_channel<rgb_in> attenuation_chan5;
  ac_channel<rgb_in> attenuation_chan6;
  ac_channel<rgb_in> attenuation_chan7;
  ac_channel<rgb_in> attenuation_chan8;
  ac_channel<rgb_in> attenuation_chan9;

  ac_channel<sphere_hittable> sphere_in1;
  ac_channel<sphere_hittable> sphere_in2;
  ac_channel<sphere_hittable> sphere_in3;
  ac_channel<sphere_hittable> sphere_in4;
  ac_channel<sphere_hittable> sphere_in5;
  ac_channel<sphere_hittable> sphere_in6;
  ac_channel<sphere_hittable> sphere_in7;
  ac_channel<sphere_hittable> sphere_in8;

  ac_channel<quad_hittable> quad_in1;
  ac_channel<quad_hittable> quad_in2;
  ac_channel<quad_hittable> quad_in3;
  ac_channel<quad_hittable> quad_in4;
  ac_channel<quad_hittable> quad_in5;
  ac_channel<quad_hittable> quad_in6;
  ac_channel<quad_hittable> quad_in7;
  ac_channel<quad_hittable> quad_in8;
};

#endif
