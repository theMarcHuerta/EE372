#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Renderer.h"

#pragma hls_design top
class Renderer {
  public:
    Renderer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(vec3<fxp_32>& in, vec3<fxp_32>& out) {
        vec3<fxp_32> v(2.5, 2.5, 2.5);
        fxp_32 a = 2;

        fxp_32 b = 4;

        out = in.negate().add(v).mult(a).div(b);
    }
};