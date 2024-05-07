#ifndef TESTQUADBUFFER_H
#define TESTQUADBUFFER_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/QuadsBuffer.h"

#pragma hls_design top
class TestQuadBuffer {
  public:
    TestQuadBuffer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<buffer_params> &paramsIn,
                        ac_channel<quad_hittable> &quads_out)
    {
        buf.run(quads_in, paramsIn, quads_out);
    }

  private:
    QuadBuffer<17> buf;
};

#endif