#ifndef PATHTRACER_TOP
#define PATHTRACER_TOP

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "deserializer.h"
#include "QuadsBuffer.h"
#include "Renderer.h"
#include "PixelAccumulator.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

#pragma hls_design top
class Pathtracer {

  public:
    Pathtracer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ac_int<12,false>> &inputChannel,
                        ac_channel<rgb_out> &output_pxl_serial)
    {
        // will initialize the variables and all
        // find a way to find max buffer size
        paramsDeserializer.run(inputChannel, quad_buffer_params, renderer_params, accumulator_params, quad_serial);

        quadsBuffer.run(quad_serial, quad_buffer_params, quads_out);

        renderer.run(quads_out, renderer_params, pxl_sample);

        pixelAccumulator.run(accumulator_params, pxl_sample, output_pxl_serial);
    }

  private:
    ParamsDeserializer paramsDeserializer;
    QuadBuffer<MAX_QUADS_IN_BUFFER> quadsBuffer;
    RendererWrapper renderer;
    PixelAccumulator pixelAccumulator;

    ac_channel<quad_hittable> quad_serial;
    ac_channel<quad_hittable> quads_out;
    ac_channel<quad_hittable> quads_out_two;
    ac_channel<img_params> renderer_params;
    ac_channel<img_params> accumulator_params;
    ac_channel<buffer_params> quad_buffer_params;
    ac_channel<rgb_in> pxl_sample;

};

#endif
