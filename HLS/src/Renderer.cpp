#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "vec3.h"
#include "Renderer.h"

#pragma hls_design top
class Renderer {
  public:
    Renderer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(vec3<fxp_32>& in, vec3<fxp_32>& out) {
        vec3<fxp_32> v(8.5, 8.5, 8.5);
        out = in.negate().add(v);
    }
};