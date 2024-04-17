#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class ShaderCores
{
public:
    ShaderCores(){}
    
#pragma hls_design interface
    void run(ac_channel<sphere_hittable> &spheres_in,
             ac_channel<quad_hittable> &quads_in, 
             ac_channel<ray> &ray_in,
             ac_channel<LoopIndices> &loop_in,
             ac_channel<img_params> &params_in,
             ac_channel<rgb_t<uint_8>> &pxl_color_out
            )
    {
      // pass in all variables to loop 
      // should be the ray and access to the buffer/ram holding the sphere and quad hittables/primitaives
      // this should return a stream of outputted pixels / vec3 of 8 bits
      // DO WE HAVE FIFO OR WHAT
      sphere_hittable tmp_sph;
      quad_hittable tmp_quad;
      tmp_sph = spheres_in.read();
      tmp_quad = quads_in.read();
      for (int i = 0; i < 8; i++){
        sphere_in[i].write(tmp_sph);
        quad_in[i].write(tmp_quad);
      }
      // need to have a reg or fifo 
      vec3<sfp_9_10> zeros;
      zeros.x = 0;
      zeros.y = 0;
      zeros.z = 0;
      vec3<sfp_9_10> ones;
      ones.x = 1<<10;
      ones.y = 1<<10;
      ones.z = 1<<10;
      accumalated_color_chan[0].write(zeros);
      attenuation_chan[0].write(ones);

      shader1.run(quad_in[0], sphere_in[0], ray_in, attenuation_chan[0], accumalated_color_chan[0], attenuation_chan[1], accumalated_color_chan[1], ray2);
      shader2.run(quad_in[1], sphere_in[1], ray2, attenuation_chan[1], accumalated_color_chan[1], attenuation_chan[2], accumalated_color_chan[2], ray3);
      shader3.run(quad_in[2], sphere_in[2], ray3, attenuation_chan[2], accumalated_color_chan[2], attenuation_chan[3], accumalated_color_chan[3], ray4);
      shader4.run(quad_in[3], sphere_in[3], ray4, attenuation_chan[3], accumalated_color_chan[3], attenuation_chan[4], accumalated_color_chan[4], ray5);
      shader5.run(quad_in[4], sphere_in[4], ray5, attenuation_chan[4], accumalated_color_chan[4], attenuation_chan[5], accumalated_color_chan[5], ray6);
      shader6.run(quad_in[5], sphere_in[5], ray6, attenuation_chan[5], accumalated_color_chan[5], attenuation_chan[6], accumalated_color_chan[6],ray7);
      shader7.run(quad_in[6], sphere_in[6], ray7, attenuation_chan[6], accumalated_color_chan[6], attenuation_chan[7], accumalated_color_chan[7],ray8);
      shader8.run(quad_in[7], sphere_in[7], ray8, attenuation_chan[7], accumalated_color_chan[7], atten_out, color_out, ray_dummy_out);

      for (int i=0; i<8; i++){
        // unroll this
        while(accumalated_color_chan[i].available()){
          vec3<sfp_9_10> tmp_color;
          tmp_color= color_out.read();
          rgb_t<uint_8> trunc_color;
          trunc_color.r = tmp_color.r >> 10; // USE BIT SLICING PARAMS HERE
          trunc_color.g = tmp_color.g >> 10;
          trunc_color.b = tmp_color.b >> 10;
          pxl_color_out.write(trunc_color)
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

  ac_channel<ray> ray2;
  ac_channel<ray> ray3;
  ac_channel<ray> ray4;
  ac_channel<ray> ray5;
  ac_channel<ray> ray6;
  ac_channel<ray> ray7;
  ac_channel<ray> ray8;
  ac_channel<ray> ray_dummy_out;

  ac_channel<rgb_t<sfp_9_10>> accumalated_color_chan[8];

  ac_channel<rgb_t<sfp_9_10>> attenuation_chan[8];

  ac_channel<rgb_t<sfp_9_10>> color_out;

  ac_channel<rgb_t<sfp_9_10>> atten_out;

  ac_channel<sphere_hittable> sphere_in[8];
  ac_channel<quad_hittable> quad_in[8];

  vec3<sfp_9_10> accum_regs[8];
  vec3<sfp_9_10> atten_regs[8];
};

