#ifndef TESTRAYGEN_H
#define TESTRAYGEN_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/PixelAccumulator.h"

#pragma hls_design top
class TestPixelAccum {
  public:
    TestPixelAccum() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<img_params> &accumulator_parms,
                        ac_channel<rgb_in> &pxl_sample,
                        ac_channel<rgb_out> &output_pxl_serial)
    {
        pixel_accum.run(accumulator_parms, pxl_sample, output_pxl_serial);
    }

  private:
    PixelAccumulator pixel_accum;
};

#endif