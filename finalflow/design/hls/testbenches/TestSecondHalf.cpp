#ifndef TESTSECONDHALF_H
#define TESTSECONDHALF_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/PixelAccumulator.h"
#include "../src/Renderer.h"

#pragma hls_design top
class TestSecondHalf {
  public:
    TestSecondHalf() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<quad_hittable>& quads_two,
                        ac_channel<img_params> &render_params,
                        ac_channel<img_params> &accumulator_parms,
                        ac_channel<rgb_out> &output_pxl_serial)
    {
        renderer.run(quads_in, quads_two, render_params, samples);
        pixel_accum.run(accumulator_parms, samples, output_pxl_serial);
    }

  private:
    RendererWrapper renderer;
    PixelAccumulator pixel_accum;

    ac_channel<rgb_in> samples;
};

#endif