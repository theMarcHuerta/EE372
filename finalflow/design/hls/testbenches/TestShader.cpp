#ifndef TESTSHADER_H
#define TESTSHADER_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/Shader.h"

#pragma hls_design top
class TestShader {

  public:
    TestShader() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<buffer_obj_count> &params_in,
                        ac_channel<quad_hittable> &quad_in,
                        ac_channel<ray> &ray_in,
                        ac_channel<rgb_in> &attenuation_chan_in,
                        ac_channel<rgb_in> &accumalated_color_chan_in,
                        ac_channel<rgb_in> &attenuation_chan_out,
                        ac_channel<rgb_in> &accumalated_color_out,
                        ac_channel<ray> &ray_out) {

        core.run(params_in, quad_in, ray_in, attenuation_chan_in, accumalated_color_chan_in, 
                            attenuation_chan_out, accumalated_color_out, ray_out);
    } 

  private:
    Shader core;

};

#endif