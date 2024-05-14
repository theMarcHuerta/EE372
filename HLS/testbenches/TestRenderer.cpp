#ifndef TESTRENDERER_H
#define TESTRENDERER_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/Renderer.h"

#pragma hls_design top
class TestRenderer {

  public:
    TestRenderer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<img_params> &render_params,
                        ac_channel<rgb_in> &output_pxl_sample) {

        render.run(quads_in, render_params, output_pxl_sample);
    } 

  private:
    RendererWrapper render;

};

#endif