#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Renderer.h"

#pragma hls_design top
class TestInterval {
  public:
    TestInterval() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(sfp_11_22& lowest, sfp_11_22& highest) {
        interval<sfp_11_22> x;
        lowest = x.max;
        highest = x.min;
    }
};