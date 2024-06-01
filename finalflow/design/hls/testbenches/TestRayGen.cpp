#ifndef TESTRAYGEN_H
#define TESTRAYGEN_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/RayGeneration.h"

#pragma hls_design top
class TestRayGen {
  public:
    TestRayGen() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<LoopIndices> &loopIndicesIn,
                        ac_channel<img_params> &paramsIn, // do i need to make it so i can fetch on command
                        ac_channel<buffer_obj_count> &paramsOut,
                        ac_channel<ray> &rayOut)
    {
        ray_gen.run(loopIndicesIn, paramsIn, paramsOut, rayOut);
    }

  private:
    RayGeneration ray_gen;
};

#endif