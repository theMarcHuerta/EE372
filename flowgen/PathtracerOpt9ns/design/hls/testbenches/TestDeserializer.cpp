#ifndef TESTDESERIALIZER_H
#define TESTDESERIALIZER_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/deserializer.h"

#pragma hls_design top
class TestDeserializer {

  public:
    TestDeserializer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ac_int<12,false>> &inputChannel,
                        ac_channel<buffer_params> &qbuffer_params,
                        ac_channel<img_params> &render_params,
                        ac_channel<img_params> &accum_params,
                        ac_channel<quad_hittable> &quad_serial_out
                        ) 
    {

        DUT.run(inputChannel, qbuffer_params, render_params, accum_params, quad_serial_out);
    } 

  private:
    ParamsDeserializer DUT;

};

#endif