#ifndef TESTSHADERCORES_H
#define TESTSHADERCORES_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/ShaderCores.h"

#pragma hls_design top
class TestShaderCores {

  public:
    TestShaderCores() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<ray> &ray_in,
                        ac_channel<buffer_obj_count> &params_in,
                        ac_channel<rgb_in> &output_pxl_serial) {

        core.run(quads_in, ray_in, params_in, output_pxl_serial);
    } 

  private:
    ShaderCores core;

};

#endif