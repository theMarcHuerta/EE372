#ifndef RTCORE_CPP
#define RTCORE_CPP


#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "RTcore.h"
#include "deserializer.h"
#include "QuadsBuffer.h"
#include "Renderer.h"
#include "PixelAccumulator.h"

#include <mc_scverify.h>

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_channel.h>



#pragma hls_design top
class RT_core{
public:
    RT_core(){}

#pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ac_int<12,false>> &input_channel, 
                        ac_channel<rgb_out> &output_pxl_serial)
    {
        // will initialize the variables and all
        // find a way to find max buffer size
        paramsDeserializer.run(input_channel, quad_buffer_params, renderer_params, accumulator_params, quad_serial);

        quadsBuffer.run(quad_serial, quad_buffer_params, quads_out);

        renderer.run(quads_out, renderer_params, pxl_sample);

        pixelAccumulator.run(accumulator_params, pxl_sample, output_pxl_serial);
        // HAVE AN ACCUMULATOR 
    }

private:
    ParamsDeserializer paramsDeserializer;
    QuadsBuffer<MAX_QUADS_IN_BUFFER> quadsBuffer;
    RendererWrapper renderer;
    PixelAccumulator pixelAccumulator;

    ac_channel<ac_int<12,false>> quad_serial;
    ac_channel<quad_hittable> quads_out;
    ac_channel<img_params> renderer_params;
    ac_channel<img_params> accumulator_params;
    ac_channel<buffer_params> quad_buffer_params;
    ac_channel<rgb_in> pxl_sample;
};

#endif
